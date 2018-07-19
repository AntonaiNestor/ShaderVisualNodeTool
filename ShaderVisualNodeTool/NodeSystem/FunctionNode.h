#pragma once
#include "Node.h"




class FunctionNode : public Node
{
public:

	FunctionNode();
	//FunctionNode(std::string NodeName,std::vector<SlotInformation> slots,std::vector<int> AllowedShaders,std::string functionCode);
	FunctionNode(FunctionNodeInformation nodeInfo);
	~FunctionNode();


	std::string StringCode;

	//methods
	void Compile(std::shared_ptr<Node> root);
	std::string CodeString();

	//can't have these. We do not run the actual code that calculates the output.
	//The only for this to be functional is to have a compute shader for every 
	// function Node
	void CalculateOutput();
	void UpdateInputs();

};

