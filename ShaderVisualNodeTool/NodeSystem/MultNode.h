#pragma once
#include "Node.h"
class MultNode :
	public Node
{
public:
	MultNode();
	~MultNode();


	//The current output value of the node
	//This is only one for now since i know have only one output
	float Value;

	//do i need these?
	short MaxNoInputs;
	short MaxNoOutputs;
	// default value ?


	// the input and output variable names?
	// 
	std::vector<std::string> varName;

	//methods -- Do i need to pass shadercode by reference?
	void Compile(std::string *ShaderCode);
	void DrawNode(bool active);

	//this is a method for all different numerical function nodes, it will implement the
	// function of the node / output , according to the inputs
	float CalculateValue();

	// codestring should be probably in the parent node as well since all nodes have it
	std::string CodeString();
	//float NodeValue();

};

