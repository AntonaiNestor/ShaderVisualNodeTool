#include "OutputNode.h"



OutputNode::OutputNode(std::string name , int NoInputs , std::string functionCode)
{

	//general node attributes
	Name = name;
	Type = BaseNodeType::OutputNode;
	UniqueID = Graph::getInstance()->AssignID();

	finalFragment = functionCode;
	HasCompiled = false;

	std::string strArray[]{ "$r","$g","$b","$a" };
	//output struct creation


	for (int i = 0; i < NoInputs; i++) {
		Connection DefaultIn;
		DefaultIn.Name = strArray[i];
		DefaultIn.DataType = Float;
		DefaultIn.Value = 1; // Not correct, the output will never be actually calculated at this point. The code never runs 
							 // You have to attach the full code to a shader, that is how you get a value
							 // But for now keep it

		Input.push_back(DefaultIn);
	}


	
}


OutputNode::~OutputNode()
{
}

void OutputNode::Compile(std::string * ShaderCode)
{
	int counter = 0;
	std::string tempCode = finalFragment;
	std::string strArray[]{ "$r","$g","$b","$a" };
	auto Manager = Graph::getInstance();
	int found = 0;


	for (int i = 0; i < Input.size(); i++) {
		//if the input isn;t connected to anything , replace name of the variable with default value
		if (Input.at(i).ConnectedNode) {
			auto SlotName = std::to_string(Input.at(i).ConnectedNode->UniqueID) + "->" + std::to_string(Input.at(i).ConnectionIndex);
			tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, strArray[i], Manager->VarToSlotMap[SlotName]);
			//ShaderCode->append("\n" +Input.at(i).Name + " = " + std::to_string(Input.at(0).Value) + ";");
		}
		else {
			tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, strArray[i], std::to_string(Input.at(i).Value));
		}

	}


	ShaderCode->append("\n" + tempCode + CodeString());
	std::cout << *ShaderCode << std::endl;
	HasCompiled = true;
}

std::string OutputNode::CodeString()
{

	return std::string(" \n //End of appending \n ");
}
