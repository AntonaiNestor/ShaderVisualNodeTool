#pragma once
#include "Node.h"


class OutputNode : public Node
{
public:
	OutputNode(std::string Name,int NoInputs,std::string functionCode);
	~OutputNode();

	std::string finalFragment;
	std::string shaderCode;

	void Compile(std::string *ShaderCode);
	std::string CodeString();
};

