#include "ArrayIndex.h"
#include "OutputNode.h"


ArrayIndex::ArrayIndex(FunctionNodeInformation nodeInfo):FunctionNode(nodeInfo)
{
	//calls the parent function as it is
}


ArrayIndex::~ArrayIndex()
{
}

void ArrayIndex::Compile(std::shared_ptr<Node> root)
{
	//only compile changes	

	//I believe this works for any type of array declaration. Apart from the where to write the shadercode ofcourse
	// which for now it is simply only written at MainLoopSeg
	//

	//If the incoming name is of the form xx[i] then the name generation should work

	std::string tempCode = StringCode;
	auto Manager = Graph::getInstance();

	auto SlotName = std::to_string(Input.at(0).ConnectedNode->UniqueID) + "->" + std::to_string(Input.at(0).ConnectionIndex);
	std::string uniqueName = Manager->VarToSlotMap[SlotName];

	//replace $Array with whatever it is connected or simply a default vec4
	tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, Input.at(0).Name, uniqueName);

	//find the [] of the array and insert the name of the $Index where i is
	auto bracketPos = tempCode.find("[");
	tempCode.erase(tempCode.begin() + bracketPos+1);

	SlotName =std::to_string(Input.at(1).ConnectedNode->UniqueID) + "->" + std::to_string(Input.at(1).ConnectionIndex);
	uniqueName = Manager->VarToSlotMap[SlotName];
	tempCode.insert(bracketPos+1, uniqueName);


	auto outName = Output.at(0).Name;
	auto tempOutName = outName;
	auto outSlotName = std::to_string(this->UniqueID) + "->" + std::to_string(0);
	//auto ManagerInstance = Graph::getInstance();

	outName = Manager->AssignUniqueName(outName, outSlotName);
	tempCode = Graph::getInstance()->ReplaceVarNames(tempCode, tempOutName, outName);

	//Meant only to be used in the Geometry for now
	if (CurrShaderType == GEOMETRY) {
		dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainLoopSeg, ShaderType(CurrShaderType));
	}
	else {
		dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainSeg, ShaderType(CurrShaderType));
	}

	HasCompiled = true;
}
