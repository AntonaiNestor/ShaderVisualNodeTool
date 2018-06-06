#pragma once
#include "Node.h"




class FunctionNode : public Node
{
public:

	
	FunctionNode(std::string NodeName,std::vector<SlotInformation> slots, std::string functionCode);
	~FunctionNode();


	std::string StringCode;

	//methods
	void Compile(std::string *ShaderCode);
	std::string CodeString();

	//can't have these. We do not run the actual code that calculates the output.
	//The only for this to be functional is to have a compute shader for every 
	// function Node
	void CalculateOutput();
	void UpdateInputs();

};

