#include "ConstantNode.h"
#include <iostream>

ConstantNode::ConstantNode()
{
	//general node attributes
	Name = "Constant";
	Type = NodeType::InputNode;
	UniqueID = Graph::getInstance()->AssignID();
	//varname might be important to check before assigning
	varName = "ConstantFloat";
	value = DefaultValue;
	HasCompiled = false;


	//output struct creation
	Connection connect;
	connect.ConnectedNode = nullptr;
	connect.ConnectionIndex = -1;
	connect.Value = value;
	connect.Enabled = true;

	Output.push_back(connect);


}

ConstantNode::ConstantNode(float a)
{
	//general node attributes
	Name = "Constant";
	Type = NodeType::InputNode;
	UniqueID = Graph::getInstance()->AssignID();
	//varname might be important to check before assigning
	varName = "ConstantFloat";
	value = a;
	HasCompiled = false;
	

	//output struct creation
	Connection connect;
	connect.ConnectedNode = nullptr;
	connect.ConnectionIndex = -1;
	connect.Value = a;
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
	//DrawNode(true);
}

std::string ConstantNode::CodeString()
{
	return "$" + varName + " = " + std::to_string(value) + ";";
}





