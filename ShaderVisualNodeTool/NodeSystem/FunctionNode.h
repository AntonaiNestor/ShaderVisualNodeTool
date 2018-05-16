#pragma once
#include "Node.h"




class FunctionNode : public Node
{
public:

	
	FunctionNode(std::string NodeName,int NoInputs, std::string functionCode);
	~FunctionNode();


	std::string StringCode;

	//methods
	void Compile(std::string *ShaderCode);
	std::string CodeString();

	//this is not correct being here i feel, the output 
	void CalculateOutput();
	void UpdateInputs();

};

