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
};

