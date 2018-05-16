#include "FunctionNode.h"





FunctionNode::FunctionNode(std::string NodeName,int NoInputs,std::string functionCode)
{
	//general node attributes
	Name = NodeName; 
	Type = BaseNodeType::FunctionNode;
	UniqueID = Graph::getInstance()->AssignID();
	//varname might be important to check before assigning
	//varName = "ConstantFloat";
	//value = DefaultValue;
	HasCompiled = false;
	StringCode = functionCode;

	//output struct creation

	for (int i = 0; i < NoInputs; i++) {

		Connection connect;

		//Pairs of var names and types will be passed as a list of pairs from the parser
		connect.Name = "Input " + std::to_string(i); 
		connect.DataType = Float;
		connect.Value = 1.0;

		Input.push_back(connect);
	}
	
	Connection DefaultOut;
	DefaultOut.Name = "Output";
	DefaultOut.DataType = Float;
	DefaultOut.Value = 1; // Not correct, the output will never be actually calculated at this point. The code never runs 
	// You have to attach the full code to a shader, that is how you get a value
	// But for now keep it

	Output.push_back(DefaultOut);
}


FunctionNode::~FunctionNode()
{
}

void FunctionNode::Compile(std::string * ShaderCode)
{
	//CalculateValue();

	for (int i = 0; i < Input.size();i++) {
		//if the input isn;t connected to anything append default value variable in shadercode
		if (!Input.at(i).ConnectedNode) {
			ShaderCode->append("\n" +Input.at(i).Name + " = " + std::to_string(Input.at(0).Value) + ";");
		}
	
	}
	ShaderCode->append("\n" + CodeString());

	HasCompiled = true;

}

std::string FunctionNode::CodeString()
{
	return StringCode;
}
