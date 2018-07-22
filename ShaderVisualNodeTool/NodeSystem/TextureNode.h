#pragma once
#include "InputNode.h"


class TextureNode :
	public InputNode
{
public:

	TextureNode(InputNodeInformation nodeInfo);
	~TextureNode();


	bool TextureGenerated;
	std::string FileName ;
	unsigned int TextureSlot;
	unsigned int TextureID;
	

	void LoadTexture();
	//void Compile(std::shared_ptr<Node> root);
};

