#include "ConstantNode.h"
#include <iostream>

ConstantNode::ConstantNode()
{
	//general node attributes
	Name = "Float";
	Type = BaseNodeType::InputNode;
	UniqueID = Graph::getInstance()->AssignID();
	//varname might be important to check before assigning
	//varName = "ConstantFloat";
	//value = DefaultValue;
	HasCompiled = false;


	//output struct creation
	Connection connect;
	
	//connect.Value = 1.0f;
	connect.VariableType = Float;
	connect.Name = "FloatVar";

	Output.push_back(connect);
	//Input.push_back(connect);


}

//ConstantNode::ConstantNode(float a)
//{
//	//general node attributes
//	Name = "Float";
//	Type = BaseNodeType::InputNode;
//	UniqueID = Graph::getInstance()->AssignID();
//	//varname might be important to check before assigning
//	//varName = "ConstantFloat";
//	//value = a;
//	HasCompiled = false;
//	
//
//	//output struct creation
//	Connection connect;
//	connect.ConnectedNode = nullptr;
//	connect.ConnectionIndex = -1;
//	connect.Value = a;
//	connect.VariableType = Float;
//	connect.Name = "FloatVar";
//	connect.Enabled = true;
//
//	Output.push_back(connect);
//	
//
//}

ConstantNode::ConstantNode(std::string string_type)
{

	auto graph = Graph::getInstance();
	Type = InputNode;
	UniqueID = graph->AssignID();
	HasCompiled = false;

	ValueType type = util::stringToValueType(string_type);
	//output struct creation
	Connection connect;
	connect.ConnectedNode = nullptr;
	connect.ConnectionIndex = -1;
	connect.VariableType = type;
	
	connect.Enabled = true;

	
	switch (type) {
		case (Float):{
			Name = "Float";
			connect.Name = "FloatVar";
			value.f_var = graph->DefaultFloat;
		}
			break;
		case (Int):{
			Name = "Integer";
			connect.Name = "IntVar";
			value.i_var = graph->DefaultInt;
			break;
		}
		case (Vec2):{
			Name = "Vec2";
			connect.Name = "Vec2Var";
			value.vec2_var = graph->DefaultVec2;
			break;
		}
		case (Vec3):{
			Name = "Vec3";
			connect.Name = "Vec3Var";
			value.vec3_var = graph->DefaultVec3;
			break;
		}
		case (Vec4): {
			Name = "Vec4";
			connect.Name = "Vec4Var";
			value.vec4_var = graph->DefaultVec4;
			break;
		}
		case (Mat4): {
			Name = "Mat4x4";
			connect.Name = "Mat4Var";
			value.mat4_var = graph->DefaultMat4;
			break;
		}
		case (Sampler2D):{
			Name = "Texture";
		
			break;
		}
		case(SamplerCube):{
			Name = "SamplerCube";
			break;
		}
		default:{
			Name = "Float";
			break;
		}

		

	}
	Output.push_back(connect);

}

ConstantNode::~ConstantNode()
{
	std::cout << " Destroying Constant Node " << std::endl;
}

//
// Compiling constant nodes means only appending a simple var in the shadercode
// The actual value of the node is also passed to the children nodes on connection
void ConstantNode::Compile(std::string	*ShaderCode) {
	
	ShaderCode->append("\n" + CodeString());
	HasCompiled = true;
	
}

std::string ConstantNode::CodeString()
{
	// TODO this probably needs to check if there are more than one outputs and append that much text
	// Also for now we do not add the "$"
	std::string name = Output.at(0).Name;
	std::string slotName = std::to_string(this->UniqueID) + "->0" ;
	ValueType outputType = Output.at(0).VariableType;
	//THE FOLLOWING SHOULD BE IN A FUNCTION SINCE I AM REUSING IT
	auto ManagerInstance = Graph::getInstance();
	try {
		//if the name exists in the map, then create a unique new name and insert that into the map.
		//Also change slot name to that new name , so that we won't have to do this all the time
		ManagerInstance->SlotToVariableMap.at(name);
		//Since we are reseting the namecounter and the map, there is no reason for that
		//Output.at(0).Name = name + "_"+  Graph::getInstance()->GiveName();
		name = name + "_" + Graph::getInstance()->GiveName();
		ManagerInstance->SlotToVariableMap.insert(std::pair<std::string, std::string>(name, slotName));
		ManagerInstance->VarToSlotMap.insert(std::pair<std::string, std::string>(slotName, name));

	}
	catch (std::out_of_range) {
		ManagerInstance->SlotToVariableMap.insert(std::pair<std::string, std::string>(name, slotName));
		ManagerInstance->VarToSlotMap.insert(std::pair<std::string, std::string>(slotName, name));
	}

	std::string StringVal;
	switch (outputType) {


	case(Float): {
		StringVal = "float " + name + " = " + std::to_string(value.f_var) + ";";
		break;
	}
	case(Int): {
		StringVal = "int " + name + " = " + std::to_string(value.i_var) + ";";;
		break;
	}
	case(Vec2): {
		StringVal = "vec2 " + name + " =  vec2(" + std::to_string(value.vec2_var.x) +" , " + std::to_string(value.vec2_var.y) + ") ;";
		break;

	}
	case(Vec3): {
		StringVal = "vec3 " + name + " =  vec3(" + std::to_string(value.vec3_var.x) + " , " + std::to_string(value.vec3_var.y) + " , " + std::to_string(value.vec3_var.z) + ") ;";
		break;

	}
	case(Vec4): {
		StringVal = "vec4 " + name + " =  vec4(" + std::to_string(value.vec4_var.x) + " , " + std::to_string(value.vec4_var.y) + " , " + std::to_string(value.vec4_var.z) + " , " + std::to_string(value.vec4_var.w) + ") ;";
		break;

	}
	case(Mat4): {
		StringVal = "mat4 ";
		break;
	}

	case(Sampler2D): {
		StringVal = "Sampler ";
		break;

	}
	case(SamplerCube): {
		StringVal = "SamplerCube ";
		break;

	}
	default:

		break;
	}

	return StringVal;
}





