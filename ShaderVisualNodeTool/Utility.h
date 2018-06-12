
#pragma once


#include <vector>
#include "Rendering/Mesh.h"


enum ValueType {
	DefaultSlotType,
	Bool,
	Float,
	Int,
	Vec2,
	Vec3,
	Vec4,
	Mat4,
	Sampler2D,
	SamplerCube
};



namespace util {
	
	/*float DefaultFloat = 1.0f;
	int DefaultInt = 1;
	glm::vec2 DefaultVec2 = glm::vec2(1.0f,1.0f);
	glm::vec3 DefaultVec3 = glm::vec3(1.0f,1.0f,1.0f);
	glm::vec4 DefaultVec4 = glm::vec4(1.0f,1.0f,1.0f,1.0f);
	glm::mat4 DefaultMat4 = glm::mat4();*/


	
	std::vector<Vertex> Arrays3ToVec3(std::vector<float> positions);
	std::vector<Vertex> Arrays3ToVec3Tex(std::vector<float> positions, std::vector<float> texCoords);

	std::vector<Vertex> ArraysToVec3(float positions[],float normals[]);
	std::vector<Vertex> ArraysToVec3(float positions[],float normals[],float texcoords[]);
	
	std::vector<unsigned int> ArraysToUintVec(unsigned int indices[]);

	ValueType stringToValueType(std::string strVal);
	bool stringToSlotType(std::string strSlotType);
	std::string GetStringValueType(ValueType type,bool defaultValue);
};