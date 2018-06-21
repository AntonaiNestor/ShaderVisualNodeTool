#pragma once

#include "InputNode.h"

class TimeNode :
	public InputNode
{
public:
	TimeNode();
	~TimeNode();

	//overwritting edituniform fow now
	void EditUniform();

	void Compile(std::shared_ptr<Node> root);
};

