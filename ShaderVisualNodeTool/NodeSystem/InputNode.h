#pragma once

#include "Node.h"
#include <map>
//
// Node that only has outputs. Base class as well
//
class InputNode : public Node
{
public:
	//InputNode();
	//~InputNode();
	 void Compile ();

	//the float will be wrapped as a Symbol or ValueType etc
	std::map<float, Node*> Outputs; //value of the node and Node in which it is outputed
	//virtual void LoadValues();
private:
	
	
	
};

