#pragma once

#include "Imgui/imgui.h"
#include "GUIManager.h"

//replace this with const variables?
#define TITLE_BOX_HEIGHT 20
#define PADDING_X  20
#define PADDING_Y  5
#define TITLE_PADDING_X  30
#define TITLE_PADDING_Y  5

class GUIManager;

class VisualNode
{



public:


	//attributes 

	Node* GNode; // the actual node that is linked to the visual part 
	GUIManager* Manager;
	ImVec2 VNodePos;
	ImVec2 VNodeSize;

	
	//methods


	//constructors
	VisualNode(GUIManager* manager,Node* Gnode, ImVec2 position);
	~VisualNode();

	void DisplayNode(ImDrawList* drawList,ImVec2 offset);
};

