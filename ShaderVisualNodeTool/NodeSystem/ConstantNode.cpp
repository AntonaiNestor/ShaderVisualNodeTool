#include "ConstantNode.h"
#include <iostream>

ConstantNode::ConstantNode()
{
	//general node attributes
	Name = "Input Float";
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
	connect.Name = "Constant Value";

	Output.push_back(connect);
	//Input.push_back(connect);


}

ConstantNode::ConstantNode(float a)
{
	//general node attributes
	Name = "Constant";
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
	connect.Name = "Constant Value";
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
	return Output.at(0).Name + " = " + std::to_string(Output.at(0).Value) + ";";
}





