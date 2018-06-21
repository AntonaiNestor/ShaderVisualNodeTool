#pragma once


#include "Node.h"


//ShaderNodes will be nodes responsible for exposing the specific per shader attributes that 
// can be set by every different type.

//Only has inputs, no outputs

class ShaderNode : public Node
{
public:


	ShaderNode(ShaderNodeInformation nodeInfo);
	~ShaderNode();

	//which of the 5 different types is this node
	ShaderType shadeType;

	void Compile(std::shared_ptr<Node> root);
	std::string CodeString();
};

