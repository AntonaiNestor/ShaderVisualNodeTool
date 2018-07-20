#include "TextureNode.h"
#include "../Rendering/stb_image.h"


TextureNode::TextureNode(InputNodeInformation nodeInfo)
{
	auto graph = Graph::getInstance();

	Type = BaseNodeType::InputnodeT;
	inputType = InputNodeType(nodeInfo.InitInputType);
	CurrShaderType = nodeInfo.ShaderType;
	UniqueID = graph->AssignID();
	TextureID = graph->AssignTextureID();
	HasCompiled = false;
	Name = nodeInfo.Name;




	//output  slots only is a given
	for (int i = 0; i < nodeInfo.SlotNames.size(); i++) {

		//output struct creation
		OutputConnection connect;
		//connect.ConnectedNode = nullptr;
		//connect.ConnectionIndex = -1;
		connect.VariableType = nodeInfo.VarTypes[i];
		connect.Enabled = true;
		connect.Name = nodeInfo.SlotNames[i];

		//TODO : Default texture!
		//	connect.Value.mat4_var = graph->DefaultMat4;
		//	value.mat4_var = graph->DefaultMat4;
		//	break;
		//}
		//case (Sampler2D): {
		//	//Name = "Texture";

		//	break;
		//}
		//
		//connection with empty lists on connected nodes and indices
		Output.push_back(connect);
	}

	FileName = "Default.png";
	LoadTexture();

}


TextureNode::~TextureNode()
{

}




void TextureNode::LoadTexture()
{
	
	//load texture once or replace if you call this again

	auto ResPath = Graph::getInstance()->ResourcesPath;
	

	glGenTextures(1, &TextureID);
	glActiveTexture(GL_TEXTURE0 + TextureSlot);
	glBindTexture(GL_TEXTURE_2D, TextureID);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load and generate the texture
	int width, height, nrChannels;

	std::string temp = ResPath + FileName;
	unsigned char *data = stbi_load((ResPath+FileName).c_str(), &width, &height, &nrChannels, 0);
	//If data was successfully received all good, otherwise load the default and show error message
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		std::cout << "Texture Loaded" << std::endl;
	}
	else
	{
		data = stbi_load((ResPath+"Default.png").c_str(), &width, &height, &nrChannels, 0);

		if (!data) {
			std::cout << "Double Trouble " << std::endl;
		}
		std::cout << "Texture filename was not found " << std::endl;
	}
	stbi_image_free(data);

}
