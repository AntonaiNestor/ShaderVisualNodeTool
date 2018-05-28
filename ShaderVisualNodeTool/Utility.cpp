
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

std::string util::GetDefaultValue(ValueType type)
{
	std::string DefVal;
	switch (type)  {
	

	case(Float): {
		DefVal = "1.0";
		break;
	}
	case(Int): {
		DefVal = "1.0";
		break;
	}
	case(Vec2): {
		DefVal = "1.0";
		break;

	}
	case(Vec3): {
		DefVal = "1.0";
		break;

	}
	case(Vec4): {
		DefVal = "1.0";
		break;

	}
	case(Mat4): {
		DefVal = "1.0";
		break;
	}

	case(Sampler2D): {
		DefVal = "1.0";
		break;

	}
	case(SamplerCube): {
		DefVal = "1.0";
		break;

	}
	default:
		
		break;
	}

	return DefVal;
	
}


ValueType util::stringToValueType(std::string strVal) {

	if (strVal.compare("float") == 0) {
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
	else if (strVal.compare("Sampler2D") == 0) {
		return ValueType::Sampler2D;
	}
	else if (strVal.compare("SamplerCube") == 0) {
		return ValueType::SamplerCube;
	}
	else {
		return ValueType::DefaultSlotType;
	}

}


