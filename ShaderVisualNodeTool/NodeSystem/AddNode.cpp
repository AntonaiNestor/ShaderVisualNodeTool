#include "AddNode.h"



AddNode::AddNode()
{
	//general node attributes
	Name = "Add Node";
	Type = BaseNodeType::FunctionNode;
	UniqueID = Graph::getInstance()->AssignID();
	HasCompiled = false;
	MaxNoInputs = 2;
	MaxNoOutputs = 1;

	//inputs struct creation
	for (int i = 0; i < MaxNoInputs; i++) {
		Connection InputConnection;
		InputConnection.ConnectedNode = nullptr;
		InputConnection.ConnectionIndex = -1;
		InputConnection.Value = 0;
		InputConnection.Enabled = true;
		Input.push_back(InputConnection);
	}
	
	//output struct creation
	Connection OutConnection;
	OutConnection.ConnectedNode = nullptr;
	OutConnection.ConnectionIndex = -1;
	OutConnection.Value = 0;
	OutConnection.Enabled = true;

	Output.push_back(OutConnection);
}


AddNode::~AddNode()
{
}

void AddNode::Compile(std::string *ShaderCode)
{

	CalculateValue();
	ShaderCode->append("\n" + CodeString());
	
	HasCompiled = true;
}

//void AddNode::DrawNode(bool active)
//{
//}

float AddNode::CalculateValue()
{
	

	Value = 0;
	//loop through inputs and add the current values -- here it is add cause of addition node
	//Need to update the input values according to the outputs from the previous nodes
	for (std::vector<Connection>::iterator it = Input.begin(); it != Input.end(); ++it) {
		//put this in a function that all nodes have
		it->Value = it->ConnectedNode->Output.at(it->ConnectionIndex).Value;
		Value += it->Value;
	}
	//this is not correct here, patch up
	Output.at(0).Value = Value;
	return Value;
}

std::string AddNode::CodeString()
{

	//need to create var names using uniqueID + nodetype name
	//std::string code   ;

	
	/*for (std::vector<Connection>::iterator it = Input.begin(); it != Input.end(); ++it) {

		code.append("$");
	}*/

	//not correct, i don't make use of the variables values, only the names.
	return "float $AddOut1 = $AddIn1 + $AddIn2";
}
