#include "FunctionNode.h"



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

void FunctionNode::Compile(std::string* ShaderCode)
{
	//CalculateValue();

	int counter = 0;
	std::string tempCode = StringCode;

	auto Manager = Graph::getInstance();
	int found = 0;


	for (int i = 0; i < Input.size();i++) {
		//if the input isn;t connected to anything , replace name of the variable with default value
		// Otherwise, replace the name of the variable with the appropriate name of the output it is connected to.
		if (Input.at(i).ConnectedNode) {
			auto SlotName = std::to_string(Input.at(i).ConnectedNode->UniqueID) + "->"+ std::to_string(Input.at(i).ConnectionIndex);
			tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, Input.at(i).Name, Manager->VarToSlotMap[SlotName]);
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
	auto outSlotName = std::to_string(this->UniqueID) + "->0";;
	//auto ManagerInstance = Graph::getInstance();

	outName = Manager->AssignUniqueName(outName,outSlotName);
	//try {
	//	//if the name exists in the map, then create a unique new name and insert that into the map.
	//	//Also change slot name to that new name , so that we won't have to do this all the time
	//	Manager->SlotToVariableMap.at(outName);
	//	//Since we are reseting the namecounter and the map, there is no reason for that
	//	//Output.at(0).Name = name + "_"+  Graph::getInstance()->GiveName();
	//	outName = outName + "_" + Graph::getInstance()->GiveName();
	//	Manager->SlotToVariableMap.insert(std::pair<std::string, std::string>(outName, outSlotName));
	//	Manager->VarToSlotMap.insert(std::pair<std::string, std::string>(outSlotName, outName));

	//}
	//catch (std::out_of_range) {
	//	Manager->SlotToVariableMap.insert(std::pair<std::string, std::string>(outName, outSlotName));
	//	Manager->VarToSlotMap.insert(std::pair<std::string, std::string>(outSlotName, outName));
	//}

	tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, tempOutName,outName);
	
	Manager->WriteToShaderCode(tempCode,FragMain);

	//ShaderCode->append("\n" + tempCode);

	HasCompiled = true;

}

std::string FunctionNode::CodeString()
{
	return  StringCode;
}
