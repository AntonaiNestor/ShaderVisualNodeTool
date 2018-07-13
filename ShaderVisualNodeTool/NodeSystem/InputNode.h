#pragma once
#include "Node.h"



enum InputNodeType {

	ConstantVariable,
	UniformVariable,
	GlobalVariable,
	AttributeVariable,
	TransformationMatrix
};


class InputNode : public Node
{
public:
	InputNode();
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
	std::string DeclareAttribute(int index, ShaderType type);
};

