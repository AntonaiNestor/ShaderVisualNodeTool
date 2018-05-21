#include "FunctionNode.h"





FunctionNode::FunctionNode(std::string NodeName,int NoInputs,std::string functionCode)
{
	//general node attributes
	Name = NodeName; 
	Type = BaseNodeType::FunctionNode;
	UniqueID = Graph::getInstance()->AssignID();
	//varname might be important to check before assigning
	//varName = "ConstantFloat";
	//value = DefaultValue;
	HasCompiled = false;
	StringCode = functionCode;

	//output struct creation
	std::string strArray[]{ "AddResult","$a","$b" };


	for (int i = 0; i < NoInputs; i++) {

		Connection connect;

		//Pairs of var names and types will be passed as a list of pairs from the parser
		connect.Name = strArray[i+1]; 
		connect.DataType = Float;
		connect.Value = 1.0;

		Input.push_back(connect);
	}
	
	Connection DefaultOut;
	DefaultOut.Name = strArray[0];
	DefaultOut.DataType = Float;
	DefaultOut.Value = 1; // Not correct, the output will never be actually calculated at this point. The code never runs 
	// You have to attach the full code to a shader, that is how you get a value
	// But for now keep it

	Output.push_back(DefaultOut);
}


FunctionNode::~FunctionNode()
{
}

void FunctionNode::Compile(std::string* ShaderCode)
{
	//CalculateValue();

	int counter = 0;
	std::string tempCode = StringCode;
	std::string strArray[]{ "$AddResult","$a","$b" }; // mockup, this should not be used like this
	auto Manager = Graph::getInstance();
	int found = 0;


	for (int i = 0; i < Input.size();i++) {
		//if the input isn;t connected to anything , replace name of the variable with default value
		if (Input.at(i).ConnectedNode) {
			auto SlotName = std::to_string(Input.at(i).ConnectedNode->UniqueID) + "->"+ std::to_string(Input.at(i).ConnectionIndex);
			tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, strArray[i + 1], Manager->VarToSlotMap[SlotName]);
			//ShaderCode->append("\n" +Input.at(i).Name + " = " + std::to_string(Input.at(0).Value) + ";");
		}
		else {
			tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, strArray[i + 1], std::to_string(Input.at(i).Value));
		}
	
	}

	auto outName = Output.at(0).Name;
	auto outSlotName = std::to_string(this->UniqueID) + "->0";;
	//auto ManagerInstance = Graph::getInstance();
	try {
		//if the name exists in the map, then create a unique new name and insert that into the map.
		//Also change slot name to that new name , so that we won't have to do this all the time
		Manager->SlotToVariableMap.at(strArray[0]);
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
	tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, strArray[0],outName);



	ShaderCode->append("\n" + tempCode);

	HasCompiled = true;

}

std::string FunctionNode::CodeString()
{
	return  StringCode;
}
