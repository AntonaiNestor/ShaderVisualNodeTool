#include "FunctionNode.h"
#include "OutputNode.h"
#include "InputNode.h"


FunctionNode::FunctionNode()
{}
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
	for (auto slot : nodeInfo.Slots) {


		//Input slot
		if (!slot.SlotType) {

			InputConnection newConnection;

			newConnection.Name = slot.Name;
			newConnection.VariableType = slot.VarType;

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

		   Input.push_back(newConnection);
		}
		//Output Slot
		else {
			OutputConnection newConnection;

			newConnection.Name = slot.Name;
			newConnection.VariableType = slot.VarType;

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

			Output.push_back(newConnection);
		}
	}

}


FunctionNode::~FunctionNode()
{
}

void FunctionNode::Compile(std::shared_ptr<Node> root)
{

	
	std::string tempCode = StringCode;

	auto Manager = Graph::getInstance();
	
	
	//go through all inputs and replace in the string code the names of the variables
	for (int i = 0; i < Input.size();i++) {
		//if the input isn;t connected to anything , replace name of the variable with default value
		// Otherwise, replace the name of the variable with the appropriate name of the output it is connected to.
		auto tempP = Input.at(i).ConnectedNode;
		if (tempP) {
			auto SlotName = std::to_string(Input.at(i).ConnectedNode->UniqueID) + "->"+ std::to_string(Input.at(i).ConnectionIndex);

			//Here I am checking where the connected variable is coming from. If it is a varying then I need to append v or g.
			std::string shaderNamePrefix = "";

			//For function nodes, if the shadertype is not the same OR for input nodes if it is attribute variable
			
			if ((tempP->Type == InputnodeT && dynamic_cast<InputNode&>(*tempP).inputType==AttributeVariable && tempP->CurrShaderType<CurrShaderType)
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


	//TODO : make this work for multiple outputs. At the moment it will break because it will rewrite the whole code
	for (int i = 0; i < Output.size(); i++) {
		// PUT THIS UGLY THING IN A FUNCTION YOU SILLY GOOSE
		auto outName = Output.at(i).Name;
		auto tempOutName = outName;
		auto outSlotName = std::to_string(this->UniqueID) + "->"+ std::to_string(i);
		//auto ManagerInstance = Graph::getInstance();

		outName = Manager->AssignUniqueName(outName,outSlotName);
	
		//write the function code in the appropriate shader.
		// Also if this name is used in a shader stage that is greater than this, I need to also create a varying of it

		bool HasCreatedVarying = false;
		bool HasBeenDeclaredInShader = false;

		for (int connID = 0; connID < Output[i].ConnectedNode.size(); connID++) {

			if (Output[i].ConnectedNode[connID]->CurrShaderType > CurrShaderType && !HasCreatedVarying) {


				std::string stringVarType = util::GetStringValueType(Output[i].VariableType, false);
				dynamic_cast<OutputNode&>(*root).CreateVaryingPipeline(ShaderType(CurrShaderType), stringVarType, outName, " ");

				std::string prefix = Manager->GetShaderPrefix(ShaderType(CurrShaderType), true);
				
				//if the function code has not been written to the shader yet, write it and make the last line of the out to match the varying
				if (!HasBeenDeclaredInShader) {
					tempCode = Manager->ReplaceVarNames(tempCode, tempOutName, prefix + outName);
					tempCode.erase(tempCode.find(prefix + outName) - (stringVarType.size()), stringVarType.size());

					if (ShaderType(CurrShaderType) == VERTEX) {
						dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainSeg, ShaderType(CurrShaderType));

					}
					else if (CurrShaderType == GEOMETRY) {
						//the main part of the geom code is done manually
						dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainGeomSeg, ShaderType(CurrShaderType));

					}

				}
				//if it has been declared, instead of rewriting the function code which will result in errors, simply add 
				// a declaration where the varying var is = to the result already written;
				else {
					std::string code = prefix + outName + " = "+ outName;

					if (ShaderType(CurrShaderType) == VERTEX) {
						dynamic_cast<OutputNode&>(*root).WriteToShaderCode(code, MainSeg, ShaderType(CurrShaderType));

					}
					else if (CurrShaderType == GEOMETRY) {
						//the main part of the geom code is done manually
						dynamic_cast<OutputNode&>(*root).WriteToShaderCode(code, MainGeomSeg, ShaderType(CurrShaderType));

					}
					
				}

				HasCreatedVarying = true;
			}
			//Same shader
			else if (Output[i].ConnectedNode[connID]->CurrShaderType == CurrShaderType && !HasBeenDeclaredInShader) {


				if (!HasCreatedVarying){
					tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, tempOutName, outName);

					if (CurrShaderType == GEOMETRY) {
						dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainGeomSeg, ShaderType(CurrShaderType));
					}
					else {
						dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainSeg, ShaderType(CurrShaderType));
					}
				}
				else {
					std::string prefix = Manager->GetShaderPrefix(ShaderType(CurrShaderType), true);
					std::string stringVarType = util::GetStringValueType(Output[i].VariableType, false);
					std::string code = stringVarType + " " + outName + " = " + prefix + outName;

					if (CurrShaderType == GEOMETRY) {
						dynamic_cast<OutputNode&>(*root).WriteToShaderCode(code, MainGeomSeg, ShaderType(CurrShaderType));
					}
					else {
						dynamic_cast<OutputNode&>(*root).WriteToShaderCode(code, MainSeg, ShaderType(CurrShaderType));
					}
				}

				HasBeenDeclaredInShader = true;
			}


		}
		

	}


	HasCompiled = true;

}

std::string FunctionNode::CodeString()
{
	return  StringCode;
}
