#pragma once
#include "Node.h"


class OutputNode : public Node
{
public:
	OutputNode(ShaderNodeInformation nodeInfo);
	~OutputNode();


	//temporarily the output node knows both 
	std::string DefaultCode[2];
	std::string shaderCode[2];
	std::vector<std::string> CodeSections[2];
	ShaderType Stype;


	void WriteToShaderCode(std::string code, ShaderSection section, ShaderType type);
	void AssembleShaderCode();
	void ClearShaderCode();
	void Compile(std::shared_ptr<Node> root);
	std::string CodeString();
};

