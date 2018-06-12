#pragma once
#include "Node.h"



enum InputNodeType {

	ConstantVariable,
	UniformVariable,
	GlobalVariable
};


class InputNode : public Node
{
public:
	InputNode();
	
	InputNode(std::string nodeName , std::string slotName, ValueType vartype);
	~InputNode();
	

	//attributes
	InputNodeType inputType;

	//methods
	void Compile(std::string *ShaderCode);
	std::string CodeString();

	//this should have a program associated with it
	//the uniform value will 
	void EditUniform();
};

