#include "InputNode.h"

#include <iostream>

InputNode::InputNode()
{
	//general node attributes
	Name = "Float";
	Type = BaseNodeType::InputnodeConst;
	inputType = InputNodeType::ConstantVariable;
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

//why am i passing a string instead of the enum?
InputNode::InputNode(std::string string_type)
{

	auto graph = Graph::getInstance();
	Type = BaseNodeType::InputnodeConst;
	inputType = InputNodeType::ConstantVariable;
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
			connect.Value.f_var = graph->DefaultFloat;
			value.f_var = graph->DefaultFloat;
		}
			break;
		case (Int):{
			Name = "Integer";
			connect.Name = "IntVar";
			connect.Value.i_var = graph->DefaultInt;
			value.i_var = graph->DefaultInt;
			break;
		}
		case (Vec2):{
			Name = "Vec2";
			connect.Name = "Vec2Var";
			connect.Value.vec2_var = graph->DefaultVec2;
			value.vec2_var = graph->DefaultVec2;
			break;
		}
		case (Vec3):{
			Name = "Vec3";
			connect.Name = "Vec3Var";
			connect.Value.vec3_var = graph->DefaultVec3;
			value.vec3_var = graph->DefaultVec3;
			break;
		}
		case (Vec4): {
			Name = "Vec4";
			connect.Name = "Vec4Var";
			connect.Value.vec4_var = graph->DefaultVec4;

			value.vec4_var = graph->DefaultVec4;
			break;
		}
		case (Mat4): {
			Name = "Mat4x4";
			connect.Name = "Mat4Var";
			connect.Value.mat4_var = graph->DefaultMat4;
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

InputNode::~InputNode()
{
	std::cout << " Destroying Constant Node " << std::endl;
}

//
// Compiling constant nodes means only appending a simple var in the shadercode
// The actual value of the node is also passed to the children nodes on connection
void InputNode::Compile(std::string	*ShaderCode) {
	
	//Put cases here for uniforms and whatnot
	auto Manager = Graph::getInstance();

	if (Type == InputnodeUniform) {
		Manager->WriteToShaderCode(CodeString(),FragUniform);
		
	}
	else {
		Manager->WriteToShaderCode(CodeString(), FragConstant);
		//ShaderCode->append("\n" + CodeString());
	}
	
	HasCompiled = true;
	
}

std::string InputNode::CodeString()
{
	// TODO this probably needs to check if there are more than one outputs and append that much text
	// Also for now we do not add the "$"
	std::string name = Output.at(0).Name;
	std::string slotName = std::to_string(this->UniqueID) + "->0" ;
	ValueType outputType = Output.at(0).VariableType;
	auto ManagerInstance = Graph::getInstance();



	name = ManagerInstance->AssignUniqueName(name,slotName);

	std::string StringVal;

	//for now just declare them in the main
	if (Type == InputnodeUniform) {

		StringVal += "uniform ";
		switch (outputType) {


		case(Bool): {
			StringVal += "bool " + name + ";";
			break;
		}

		case(Float): {
			StringVal += "float " + name +  ";" ;
			break;
		}
		case(Int): {
			StringVal += "int " + name +  ";";
			break;
		}
		case(Vec2): {
			StringVal += "vec2 " + name + ";";
			break;

		}
		case(Vec3): {
			StringVal += "vec3 " + name + ";";
			break;

		}
		case(Vec4): {
			StringVal += "vec4 " + name + " ;";
			break;

		}
		case(Mat4): {
			StringVal += "mat4 ";
			break;
		}

		case(Sampler2D): {
			StringVal += "Sampler ";
			break;

		}
		case(SamplerCube): {
			StringVal += "SamplerCube ";
			break;

		}
		default:

			break;
		}
		
	}
	else {
		switch (outputType) {


		case(Bool): {
			StringVal += "bool " + name + " = " + std::to_string(value.b_var) + ";";
			break;
		}

		case(Float): {
			StringVal += "float " + name + " = " + std::to_string(value.f_var) + ";";
			break;
		}
		case(Int): {
			StringVal += "int " + name + " = " + std::to_string(value.i_var) + ";";;
			break;
		}
		case(Vec2): {
			StringVal += "vec2 " + name + " =  vec2(" + std::to_string(value.vec2_var.x) + " , " + std::to_string(value.vec2_var.y) + ") ;";
			break;

		}
		case(Vec3): {
			StringVal += "vec3 " + name + " =  vec3(" + std::to_string(value.vec3_var.x) + " , " + std::to_string(value.vec3_var.y) + " , " + std::to_string(value.vec3_var.z) + ") ;";
			break;

		}
		case(Vec4): {
			StringVal += "vec4 " + name + " =  vec4(" + std::to_string(value.vec4_var.x) + " , " + std::to_string(value.vec4_var.y) + " , " + std::to_string(value.vec4_var.z) + " , " + std::to_string(value.vec4_var.w) + ") ;";
			break;

		}
		case(Mat4): {
			StringVal += "mat4 ";
			break;
		}

		case(Sampler2D): {
			StringVal += "Sampler ";
			break;

		}
		case(SamplerCube): {
			StringVal += "SamplerCube ";
			break;

		}
		default:

			break;
		}
	}

	

	return StringVal;
}

void InputNode::EditUniform()
{
	//Program ID , this needs to be passed as a parameter maybe
	auto ManagerInstance = Graph::getInstance();
	Shader* program = ManagerInstance->daShader;
	ValueType outputType = Output.at(0).VariableType;
	std::string name = Output.at(0).Name;
	std::string slotName = std::to_string(this->UniqueID) + "->0";
	std::string uniqueOutName = ManagerInstance->AssignUniqueName(name,slotName);

	program->Use();

	unsigned int uloc = glGetUniformLocation(program->ID, uniqueOutName.c_str());

	switch (outputType) {


	case(Bool): {
		//bool value = Output[0].Value.b_var;
		glUniform1i(uloc,value.b_var);
		
		break;
	}

	case(Float): {
		//float value = Output[0].Value.f_var;
		glUniform1f(uloc, value.f_var);
		break;
	}
	case(Int): {
		//int value = Output[0].Value.i_var;
		glUniform1i(uloc, value.i_var);
		break;
	}
	case(Vec2): {
		//glm::vec2 value= Output[0].Value.vec2_var;
		glUniform2f(uloc,value.vec2_var.x,value.vec2_var.y);
		break;

	}
	case(Vec3): {
		//glm::vec3 value = Output[0].Value.vec3_var;
		glUniform3f(uloc, value.vec3_var.x, value.vec3_var.y,value.vec3_var.z);
		break;

	}
	case(Vec4): {
		//glm::vec4 value = Output[0].Value.vec4_var;
		glUniform4f(uloc, value.vec4_var.x, value.vec4_var.y, value.vec4_var.z,value.vec4_var.w);
		break;

	}
	case(Mat4): {
		//glm::mat4 value = Output[0].Value.mat4_var;
		glUniformMatrix4fv(uloc,1,GL_FALSE ,glm::value_ptr(value.mat4_var));
		break;
	}

	case(Sampler2D): {
		break;

	}
	case(SamplerCube): {
		break;

	}
	default:

		break;
	}

	glUseProgram(0);

}





