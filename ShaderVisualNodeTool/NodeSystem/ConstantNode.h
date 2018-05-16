#pragma once
#include "Node.h"



class ConstantNode : public Node
{
public:
	ConstantNode();
	ConstantNode(float a);
	~ConstantNode();

	//attributes
	//float value;
	float DefaultValue = 1.0;
	//std::string varName;

	//visual attributes
	

	//methods
	void Compile(std::string *ShaderCode);
	std::string CodeString();


	
	//float NodeValue();
};

