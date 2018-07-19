#pragma once
#include "FunctionNode.h"


class ArrayIndex :
	public FunctionNode


{
public:

	ArrayIndex(FunctionNodeInformation nodeInfo);
	~ArrayIndex();
	
	//methods
	void Compile(std::shared_ptr<Node> root);
	

};

