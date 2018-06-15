#pragma once
#include "Node.h"



enum InputNodeType {

	ConstantVariable,
	UniformVariable,
	GlobalVariable,
	AttributeVariable
};


class InputNode : public Node
{
public:
	InputNode();
	
	//InputNode(std::string nodeName , std::string slotName, ValueType vartype);
	InputNode(InputNodeInformation nodeInfo);
	~InputNode();
	

	//attributes
	InputNodeType inputType;

	//methods
	void Compile(std::shared_ptr<Node> root);
	std::string CodeString();

	//this should have a program associated with it
	//the uniform value will 
	void EditUniform();
};

