#pragma once
#include "Node.h"


class OutputNode : public Node
{
public:
	OutputNode(ShaderNodeInformation nodeInfo);
	~OutputNode();


	//temporarily the output node knows all three shaders 
	std::string DefaultCode[3];
	std::string shaderCode[3];
	std::vector<std::string> CodeSections[3]; //array of vectors
	ShaderType Stype;

	std::string CreateVaryingPipeline(ShaderType type ,std::string varType, std::string varName,std::string assignValue );
	void WriteToShaderCode(std::string code, ShaderSection section, ShaderType type);
	void AssembleShaderCode();
	void ClearShaderCode();
	void Compile(std::shared_ptr<Node> root);
	std::string CodeString();
};

