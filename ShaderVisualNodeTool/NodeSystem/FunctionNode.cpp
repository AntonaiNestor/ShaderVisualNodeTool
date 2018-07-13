#include "FunctionNode.h"
#include "OutputNode.h"
#include "InputNode.h"


//std::string NodeName,std::vector<SlotInformation> slots,std::string functionCode

FunctionNode::FunctionNode(FunctionNodeInformation nodeInfo)
{
	//general node attributes
	Name = nodeInfo.Name; 
	Type = BaseNodeType::FunctionnodeT;
	auto graph = Graph::getInstance();
	UniqueID = graph->AssignID();
	HasCompiled = false;
	StringCode = nodeInfo.Code;

	AllowedExecShaderTypes = nodeInfo.AllowedExecShaders;
	CurrShaderType = AllowedExecShaderTypes.back();
	//parse slots and create necessary information
	//std::vector<std::string> slotNames;

	for (auto slot : nodeInfo.Slots) {

		Connection newConnection;

		newConnection.Name = slot.Name;
		newConnection.VariableType = slot.VarType;

	

		/*std::string temp = util::GetStringValueType(slot.VarType,true);
		newConnection.Value.f_var = std::stof(temp);*/
		

	  switch (slot.VarType) {

		 case (Bool): {
			 newConnection.Value.b_var = graph->DefaultBool;
			 break;
		 }
		case(Float): {
			newConnection.Value.f_var = { graph->DefaultFloat };
			break;
		}
		case(Int): {
			newConnection.Value.i_var = { graph->DefaultInt };
			break;
		}
		case(Vec2): {
			newConnection.Value.vec2_var = { graph->DefaultVec2 };
			break;

		}
		case(Vec3): {
			newConnection.Value.vec3_var = { graph->DefaultVec3 };
			break;

		}
		case(Vec4): {
			newConnection.Value.vec4_var = { graph->DefaultVec4 };
			break;

		}
		case(Mat4): {
			newConnection.Value.mat4_var = { graph->DefaultMat4 };
			break;
		}

		default:
			break;
	  }

		//Input slot
		if (!slot.SlotType) {
			Input.push_back(newConnection);
		}
		//Output Slot
		else {
			Output.push_back(newConnection);
		}
	}

	////output struct creation
	//std::string strArray[]{ "AddResult","$a","$b" };


	//for (int i = 0; i < NoInputs; i++) {

	//	Connection connect;

	//	//Pairs of var names and types will be passed as a list of pairs from the parser
	//	connect.Name = strArray[i+1]; 
	//	connect.DataType = Float;
	//	connect.Value = 1.0;

	//	Input.push_back(connect);
	//}
	//
	//Connection DefaultOut;
	//DefaultOut.Name = strArray[0];
	//DefaultOut.DataType = Float;
	//DefaultOut.Value = 1; // Not correct, the output will never be actually calculated at this point. The code never runs 
	//// You have to attach the full code to a shader, that is how you get a value
	//// But for now keep it

	//Output.push_back(DefaultOut);
}


FunctionNode::~FunctionNode()
{
}

void FunctionNode::Compile(std::shared_ptr<Node> root)
{

	int counter = 0;
	std::string tempCode = StringCode;

	auto Manager = Graph::getInstance();
	int found = 0;
	
	//go through all inputs and replace in the string code the names of the variables
	for (int i = 0; i < Input.size();i++) {
		//if the input isn;t connected to anything , replace name of the variable with default value
		// Otherwise, replace the name of the variable with the appropriate name of the output it is connected to.
		if (Input.at(i).ConnectedNode) {
			auto SlotName = std::to_string(Input.at(i).ConnectedNode->UniqueID) + "->"+ std::to_string(Input.at(i).ConnectionIndex);

			//Here I am checking where the connected variable is coming from. If it is a varying then I need to append v or g.
			std::string shaderNamePrefix = "";

			//For function nodes, if the shadertype is not the same OR for input nodes if it is attribute variable
			auto tempP = Input.at(i).ConnectedNode;
			if ((tempP->Type == InputnodeT && dynamic_cast<InputNode&>(*tempP).inputType==AttributeVariable && CurrShaderType>VERTEX)
				|| (tempP->Type == FunctionnodeT && tempP->CurrShaderType<CurrShaderType)) {
				shaderNamePrefix = Manager->GetShaderPrefix( ShaderType(CurrShaderType), true);
				
			}
			//prefix + unique name associated with slot
			auto newName = shaderNamePrefix + Manager->VarToSlotMap[SlotName];
			
			//replace the name of the variable in the code with the added prefix. If the variable isn't a varying then the prefix does nothing
			tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, Input.at(i).Name, newName );
			Input.at(i).Value = Input.at(i).ConnectedNode->Output.at(Input.at(i).ConnectionIndex).Value;
			//ShaderCode->append("\n" +Input.at(i).Name + " = " + std::to_string(Input.at(0).Value) + ";");
		}
		else {
			//bool is a unique case because it needs special replacement if it is not connected to anything 
			if (Input[i].VariableType != Bool) {
				tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, Input.at(i).Name, util::GetStringValueType(Input[i].VariableType, true));
			}
			else {

				if (Input[i].Value.b_var == false) {
				
					tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, Input.at(i).Name,"false");
				}
				else {
					tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, Input.at(i).Name, "true");
					
				}
			
			}
			
		}
	
	}

	// PUT THIS UGLY THING IN A FUNCTION YOU SILLY GOOSE
	auto outName = Output.at(0).Name;
	auto tempOutName = outName;
	auto outSlotName = std::to_string(this->UniqueID) + "->0";
	//auto ManagerInstance = Graph::getInstance();

	outName = Manager->AssignUniqueName(outName,outSlotName);
	
	//write the function code in the appropriate shader. 

	//If that shader is the vertex then we need to introduce varyings in both,

	//TODO VARYING HERE AS WELL


	//if there node connected to the output "next" node, does not have the same shadertype (greater and not equal)
	//then create a varying pipeline for the output if need
	if (Output.at(0).ConnectedNode->CurrShaderType > CurrShaderType) {

		if (ShaderType(CurrShaderType) == VERTEX ) {

			std::string stringVarType = util::GetStringValueType(Output[0].VariableType, false);

			//std::string VertName = "out " + stringVarType + outName + " ;";
			//std::string FragName = "in " + stringVarType + outName + " ;";

			//dynamic_cast<OutputNode&>(*root).WriteToShaderCode(VertName, VaryingSeg,VERTEX);
			//dynamic_cast<OutputNode&>(*root).WriteToShaderCode(FragName, VaryingSeg, FRAGMENT);

			//creates the series of name declaration in the  varying segments of VS,GS,FS
			dynamic_cast<OutputNode&>(*root).CreateVaryingPipeline(VERTEX, stringVarType, outName, " ");

			//this might cause errors :(
			//delete the variable declaration of the output, since it is already declared in the varying section
			// This seems actually correct to do. It should be done in all shaders
			// Add the prefix v to the output name since it is a varying. and write it in the main segment of the shader you are 
			tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, tempOutName, "v" + outName);
			tempCode.erase(tempCode.find("v" + outName) - (stringVarType.size()), stringVarType.size());

			dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainSeg, ShaderType(CurrShaderType));

		}
		else if (CurrShaderType == GEOMETRY) {

			std::string stringVarType = util::GetStringValueType(Output[0].VariableType, false);
			//declaration of vars in geom and fragment 
			dynamic_cast<OutputNode&>(*root).CreateVaryingPipeline(GEOMETRY, stringVarType, outName," ");
			tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, tempOutName, "g"+outName);
			tempCode.erase(tempCode.find("g" + outName) - (stringVarType.size()), stringVarType.size());
			//the main part of the geom code is done manually
			dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainGeomSeg, ShaderType(CurrShaderType));

		} 
	}
	//Same shader
	else {
		tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, tempOutName, outName);
		if (CurrShaderType == GEOMETRY) {
			dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainGeomSeg, ShaderType(CurrShaderType));
		}
		else {
			dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainSeg, ShaderType(CurrShaderType));
		}
		
	}


	HasCompiled = true;

}

std::string FunctionNode::CodeString()
{
	return  StringCode;
}
