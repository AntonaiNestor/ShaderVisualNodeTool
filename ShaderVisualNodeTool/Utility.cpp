
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



