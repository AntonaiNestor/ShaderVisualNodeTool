#include "FunctionNode.h"





FunctionNode::FunctionNode(std::string NodeName,std::vector<SlotInformation> slots,std::string functionCode)
{
	//general node attributes
	Name = NodeName; 
	Type = BaseNodeType::FunctionNode;
	auto graph = Graph::getInstance();
	UniqueID = graph->AssignID();
	HasCompiled = false;
	StringCode = functionCode;

	//parse slots and create necessary information
	//std::vector<std::string> slotNames;

	for (auto slot : slots) {

		Connection newConnection;

		newConnection.Name = slot.Name;
		newConnection.VariableType = slot.VarType;

		std::string::size_type sz;     // alias of size_t

		std::string temp = util::GetStringValueType(slot.VarType,false);
		//newConnection.Value = std::stof(temp);
		

		/* switch (slot.VarType) {


		case(Float): {
			newConnection.Value = { graph->DefaultFloat };
			break;
		}
		case(Int): {
			newConnection.Value = { graph->DefaultInt };
			break;
		}
		case(Vec2): {
			StringVal = "vec2 ";
			break;

		}
		case(Vec3): {
			StringVal = "vec3 ";
			break;

		}
		case(Vec4): {
			StringVal = "vec4 ";
			break;

		}
		case(Mat4): {
			StringVal = "mat4 ";
			break;
		}

		case(Sampler2D): {
			StringVal = "Sampler ";
			break;

		}
		case(SamplerCube): {
			StringVal = "SamplerCube ";
			break;

		}
		default:

			break;
		}*/


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
	//std::string strArray[]{ "$AddResult","$a","$b" }; // mockup, this should not be used like this
	auto Manager = Graph::getInstance();
	int found = 0;


	for (int i = 0; i < Input.size();i++) {
		//if the input isn;t connected to anything , replace name of the variable with default value
		if (Input.at(i).ConnectedNode) {
			auto SlotName = std::to_string(Input.at(i).ConnectedNode->UniqueID) + "->"+ std::to_string(Input.at(i).ConnectionIndex);
			tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, Input.at(i).Name, Manager->VarToSlotMap[SlotName]);
			Input.at(i).Value = Input.at(i).ConnectedNode->Output.at(Input.at(i).ConnectionIndex).Value;
			//ShaderCode->append("\n" +Input.at(i).Name + " = " + std::to_string(Input.at(0).Value) + ";");
		}
		else {
			//TEMPORARY 
			if (Input[i].VariableType != Bool) {
				tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, Input.at(i).Name, util::GetStringValueType(Input[i].VariableType, true));
			}
			else {
				// THIS IS BAD CHANGE IT PLEASE FUTURE ANTONY

				if (Input[i].Enabled == true) {
					tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, Input.at(i).Name, "true");
				}
				else {
					tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, Input.at(i).Name, "false");
				}
			}
			
		}
	
	}

	// PUT THIS UGLY THING IN A FUNCTION YOU SILLY GOOSE
	auto outName = Output.at(0).Name;
	auto tempOutName = outName;
	auto outSlotName = std::to_string(this->UniqueID) + "->0";;
	//auto ManagerInstance = Graph::getInstance();
	try {
		//if the name exists in the map, then create a unique new name and insert that into the map.
		//Also change slot name to that new name , so that we won't have to do this all the time
		Manager->SlotToVariableMap.at(outName);
		//Since we are reseting the namecounter and the map, there is no reason for that
		//Output.at(0).Name = name + "_"+  Graph::getInstance()->GiveName();
		outName = outName + "_" + Graph::getInstance()->GiveName();
		Manager->SlotToVariableMap.insert(std::pair<std::string, std::string>(outName, outSlotName));
		Manager->VarToSlotMap.insert(std::pair<std::string, std::string>(outSlotName, outName));

	}
	catch (std::out_of_range) {
		Manager->SlotToVariableMap.insert(std::pair<std::string, std::string>(outName, outSlotName));
		Manager->VarToSlotMap.insert(std::pair<std::string, std::string>(outSlotName, outName));
	}
	tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, tempOutName,outName);
	


	ShaderCode->append("\n" + tempCode);

	HasCompiled = true;

}

std::string FunctionNode::CodeString()
{
	return  StringCode;
}
