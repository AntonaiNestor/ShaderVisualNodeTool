
#pragma once


#include <vector>
#include "Rendering/Mesh.h"


enum ValueType {
	DefaultSlotType,
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
	
	
	std::vector<Vertex> Arrays3ToVec3(std::vector<float> positions);
	std::vector<Vertex> Arrays3ToVec3Tex(std::vector<float> positions, std::vector<float> texCoords);

	std::vector<Vertex> ArraysToVec3(float positions[],float normals[]);
	std::vector<Vertex> ArraysToVec3(float positions[],float normals[],float texcoords[]);
	
	std::vector<unsigned int> ArraysToUintVec(unsigned int indices[]);

	ValueType stringToValueType(std::string strVal);
	bool stringToSlotType(std::string strSlotType);
	std::string GetDefaultValue(ValueType type);
};