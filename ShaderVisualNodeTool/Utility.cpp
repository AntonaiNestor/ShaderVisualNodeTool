
#include "Utility.h"

#include <iostream>
std::vector<Vertex> util::Arrays3ToVec3(std::vector<float> VertexInfo)
{
	std::vector<Vertex> VertVector;
	
	
	for (int i = 0; i < VertexInfo.size();i=i+5) {

		Vertex vert;
		vert.Position = glm::vec3(VertexInfo[i], VertexInfo[i+1], VertexInfo[i+2]);
		vert.TexCoords = glm::vec2(VertexInfo[i + 3], VertexInfo[i + 4]);
		VertVector.push_back(vert);
	}
	return VertVector;
}

bool util::stringToSlotType(std::string strSlotType)
{
	if (strSlotType.compare("Input") == 0) {
		return false;
	}
	else return true;
}

std::string util::GetStringValueType(ValueType type, bool defaultValue)
{
	std::string StringVal;
	
	//returns the string name of the variable type
	if (!defaultValue) {
	
		switch (type) {

		case(Bool): {
			StringVal = "bool";
			break;
		}

		case(Float): {
			StringVal = "float ";
			break;
		}
		case(Int): {
			StringVal = "int ";
			break;
		}
		case(Vec2): {
			StringVal = "vec2 ";
			break;

		}
		case(Vec3): {
			StringVal = "vec3 ";
			break;

		}
		case(Vec4): {
			StringVal = "vec4 ";
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
	else {
		switch (type) {

		case(Bool): {
			StringVal = "false";
			break;
		}
		case(Float): {
			StringVal = "1.0f ";
			break;
		}
		case(Int): {
			StringVal = "1";
			break;
		}
		case(Vec2): {
			StringVal = "vec2(1.0f,1.0f) ";
			break;

		}
		case(Vec3): {
			StringVal = "vec3(1.0f,1.0f,1.0f) ";
			break;

		}
		case(Vec4): {
			StringVal = "vec4(1.0f,1.0f,1.0f,1.0f) ";
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

	
	
}


ValueType util::stringToValueType(std::string strVal) {
	//The comparison feels weird, the upper case and lower case affects a lot
	if (strVal.compare("bool") == 0) {
		return ValueType::Bool;
	}
	else if (strVal.compare("float") == 0) {
		return ValueType::Float;
	}
	else if (strVal.compare("int") == 0) {
		return ValueType::Int;
	}
	else if (strVal.compare("vec2") == 0) {
		return ValueType::Vec2;
	}
	else if (strVal.compare("vec3") == 0) {
		return ValueType::Vec3;
	}
	else if (strVal.compare("vec4") == 0) {
		return ValueType::Vec4;
	}
	else if (strVal.compare("mat4") == 0) {
		return ValueType::Mat4;
	}
	else if (strVal.compare("sampler2D") == 0) {
		return ValueType::Sampler2D;
	}
	else if (strVal.compare("samplerCube") == 0) {
		return ValueType::SamplerCube;
	}
	else {
		return ValueType::DefaultSlotType;
	}

}


