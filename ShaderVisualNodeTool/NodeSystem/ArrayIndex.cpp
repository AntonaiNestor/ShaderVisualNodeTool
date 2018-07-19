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

	dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainGeomSeg, ShaderType(CurrShaderType));


	HasCompiled = true;
}
