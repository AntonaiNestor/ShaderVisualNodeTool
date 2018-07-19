#include "ShaderNode.h"
#include "OutputNode.h"
#include "InputNode.h"

ShaderNode::ShaderNode(ShaderNodeInformation nodeInfo)
{

	//Attributes
	Name = nodeInfo.Name; 
	Type = BaseNodeType::ShadernodeT;
	shadeType = nodeInfo.ShadeType;
	CurrShaderType = shadeType;
	UniqueID = Graph::getInstance()->AssignID();
	HasCompiled = false;

	auto graph = Graph::getInstance();

	//slot creation 
	for (auto slot : nodeInfo.Slots) {

		

		////The value init here might be wrong. These are special cases. 
		////The default code will handle everything
	

		//Input slot
		if (!slot.SlotType) {
			InputConnection newConnection;

			newConnection.Name = slot.Name;
			newConnection.VariableType = slot.VarType;

			Input.push_back(newConnection);
		}
		//Output Slot
		else {
			OutputConnection newConnection;

			newConnection.Name = slot.Name;
			newConnection.VariableType = slot.VarType;

			Output.push_back(newConnection);
		}
	}
}


ShaderNode::~ShaderNode()
{

}

void ShaderNode::Compile(std::shared_ptr<Node> root)
{
	auto Manager = Graph::getInstance();
	//go through the inputs and if they are connected, write in that shadertype
	// shader the replacement value of the appropriate attribute.

	for (int i = 0; i < Input.size(); i++) {
		//if the input is connected to something
		if (Input[i].ConnectedNode != nullptr) {
			
			//the difference is usually the input variable name is replaced with the $var in the code, which uses it to set the output.
			//Here the name of the input is the name of the actual output variable and we want to equal that with the name of the incoming var name. 
			std::string tempCode =  Input.at(i).Name +" = $ ;";
			std::string shaderNamePrefix = "";

			auto tempP = Input.at(i).ConnectedNode;
			//if it 
			if ((tempP->Type == InputnodeT && dynamic_cast<InputNode&>(*tempP).inputType == AttributeVariable && CurrShaderType>VERTEX)
				|| (tempP->Type == FunctionnodeT && tempP->CurrShaderType<CurrShaderType)) {

				shaderNamePrefix = Manager->GetShaderPrefix(ShaderType(CurrShaderType), true);

			}
			//slotname for finding a unique name
			auto SlotName = std::to_string(Input.at(i).ConnectedNode->UniqueID) + "->" + std::to_string(Input.at(i).ConnectionIndex);
			tempCode = Manager->ReplaceVarNames(tempCode, "$", shaderNamePrefix + Manager->VarToSlotMap[SlotName]);

			//redundant 
			//Input.at(i).Value = Input.at(i).ConnectedNode->Output.at(0).Value;

			//write on the main function of the appropriate shader the declaration

			//stupid way of writing in the main geom seg
			if (shadeType == GEOMETRY) {
				dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainGeomSeg, shadeType);
			}
			else {
				dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainSeg, shadeType);
			}
			

		
		}
	}

}

std::string ShaderNode::CodeString()
{
	return std::string();
}
