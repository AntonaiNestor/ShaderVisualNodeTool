#include "ShaderNode.h"
#include "OutputNode.h"


ShaderNode::ShaderNode(ShaderNodeInformation nodeInfo)
{

	//Attributes
	Name = nodeInfo.Name; 
	Type = BaseNodeType::ShadernodeT;
	shadeType = nodeInfo.ShadeType;
	UniqueID = Graph::getInstance()->AssignID();
	HasCompiled = false;

	auto graph = Graph::getInstance();

	//slot creation 
	for (auto slot : nodeInfo.Slots) {

		Connection newConnection;

		newConnection.Name = slot.Name;
		newConnection.VariableType = slot.VarType;


		////The value init here might be wrong. These are special cases. 
		////The default code will handle everything
		//switch (slot.VarType) {

		//case (Bool): {
		//	newConnection.Value.b_var = graph->DefaultBool;
		//	break;
		//}
		//case(Float): {
		//	newConnection.Value.f_var = { graph->DefaultFloat };
		//	break;
		//}
		//case(Int): {
		//	newConnection.Value.i_var = { graph->DefaultInt };
		//	break;
		//}
		//case(Vec2): {
		//	newConnection.Value.vec2_var = { graph->DefaultVec2 };
		//	break;

		//}
		//case(Vec3): {
		//	newConnection.Value.vec3_var = { graph->DefaultVec3 };
		//	break;

		//}
		//case(Vec4): {
		//	newConnection.Value.vec4_var = { graph->DefaultVec4 };
		//	break;

		//}
		//case(Mat4): {
		//	newConnection.Value.mat4_var = { graph->DefaultMat4 };
		//	break;
		//}

		//default:

		//	break;
		//}

		//Input slot
		if (!slot.SlotType) {
			Input.push_back(newConnection);
		}
		//Output Slot
		else {
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
			//slotname for finding a unique name
			auto SlotName = std::to_string(Input.at(i).ConnectedNode->UniqueID) + "->" + std::to_string(Input.at(i).ConnectionIndex);
			tempCode = Manager->ReplaceVarNames(tempCode, "$", Manager->VarToSlotMap[SlotName]);

			//redundant 
			//Input.at(i).Value = Input.at(i).ConnectedNode->Output.at(0).Value;

			//write on the main function of the appropriate shader the declaration
			dynamic_cast<OutputNode&>(*root).WriteToShaderCode(tempCode, MainSeg, shadeType);

		
		}
	}

}

std::string ShaderNode::CodeString()
{
	return std::string();
}
