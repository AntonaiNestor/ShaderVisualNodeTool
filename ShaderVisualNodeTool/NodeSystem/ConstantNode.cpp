#include "ConstantNode.h"
#include <iostream>

ConstantNode::ConstantNode()
{
	//general node attributes
	Name = "Float";
	Type = BaseNodeType::InputNode;
	UniqueID = Graph::getInstance()->AssignID();
	//varname might be important to check before assigning
	//varName = "ConstantFloat";
	//value = DefaultValue;
	HasCompiled = false;


	//output struct creation
	Connection connect;
	
	connect.Value = DefaultValue;
	connect.DataType = Float;
	connect.Name = "FloatVar";

	Output.push_back(connect);
	//Input.push_back(connect);


}

ConstantNode::ConstantNode(float a)
{
	//general node attributes
	Name = "Float";
	Type = BaseNodeType::InputNode;
	UniqueID = Graph::getInstance()->AssignID();
	//varname might be important to check before assigning
	//varName = "ConstantFloat";
	//value = a;
	HasCompiled = false;
	

	//output struct creation
	Connection connect;
	connect.ConnectedNode = nullptr;
	connect.ConnectionIndex = -1;
	connect.Value = a;
	connect.DataType = Float;
	connect.Name = "FloatVar";
	connect.Enabled = true;

	Output.push_back(connect);
	

}

ConstantNode::~ConstantNode()
{
	std::cout << " Destroying Constant Node " << std::endl;
}

//
// Compiling constant nodes means only appending a simple var in the shadercode
// The actual value of the node is also passed to the children nodes on connection
void ConstantNode::Compile(std::string	*ShaderCode) {
	ShaderCode->append("\n" + CodeString());
	HasCompiled = true;
	
}

std::string ConstantNode::CodeString()
{
	// TODO this probably needs to check if there are more than one outputs and append that much text
	// Also for now we do not add the "$"
	std::string name = Output.at(0).Name;
	std::string slotName = std::to_string(this->UniqueID) + "->0" ;

	//THE FOLLOWING SHOULD BE IN A FUNCTION SINCE I AM REUSING IT
	auto ManagerInstance = Graph::getInstance();
	try {
		//if the name exists in the map, then create a unique new name and insert that into the map.
		//Also change slot name to that new name , so that we won't have to do this all the time
		ManagerInstance->SlotToVariableMap.at(name);
		//Since we are reseting the namecounter and the map, there is no reason for that
		//Output.at(0).Name = name + "_"+  Graph::getInstance()->GiveName();
		name = name + "_" + Graph::getInstance()->GiveName();
		ManagerInstance->SlotToVariableMap.insert(std::pair<std::string, std::string>(name, slotName));
		ManagerInstance->VarToSlotMap.insert(std::pair<std::string, std::string>(slotName, name));

	}
	catch (std::out_of_range) {
		ManagerInstance->SlotToVariableMap.insert(std::pair<std::string, std::string>(name, slotName));
		ManagerInstance->VarToSlotMap.insert(std::pair<std::string, std::string>(slotName, name));
	}

	return "float "+ name + " = " + std::to_string(Output.at(0).Value) + ";";
}





