#include "OutputNode.h"



OutputNode::OutputNode()
{

	//general node attributes
	Name = "Output";
	Type = BaseNodeType::OutputNode;
	UniqueID = Graph::getInstance()->AssignID();

	HasCompiled = false;


	//output struct creation

	Connection DefaultIn;
	DefaultIn.Name = "Result";
	DefaultIn.DataType = Float;
	DefaultIn.Value = 1; // Not correct, the output will never be actually calculated at this point. The code never runs 
						  // You have to attach the full code to a shader, that is how you get a value
						  // But for now keep it

	Input.push_back(DefaultIn);

	
}


OutputNode::~OutputNode()
{
}

void OutputNode::Compile(std::string * ShaderCode)
{
	ShaderCode->append(CodeString());

	HasCompiled = true;
}

std::string OutputNode::CodeString()
{
	return std::string(" \n End of appending \n ");
}
