#pragma once
#include "Node.h"


class OutputNode : public Node
{
public:
	OutputNode();
	~OutputNode();

	std::string shaderCode;

	void Compile(std::string *ShaderCode);
	std::string CodeString();
};

