#pragma once
#include "Node.h"
#include "glm/glm.hpp"
#include "../GUI/Imgui/imgui.h"


class ConstantNode : public Node
{
public:
	ConstantNode(float a, float x, float y);
	~ConstantNode();

	//attributes
	float value;
	std::string varName;

	//visual attributes

	glm::vec2 NodePos;



	//methods
	void Compile(std::string *ShaderCode);
	std::string CodeString();


	void DrawNode(bool active);
	//float NodeValue();
};

