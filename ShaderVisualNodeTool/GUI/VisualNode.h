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


struct ConnectionVCoords {

	ImVec2 SlotCoords;
	ConnectionVCoords* conn;
	//ImVec2 ConnectedCoords;
	bool connected;
	bool drawn;

	//methods
	ConnectionVCoords();
	void SetCoords(ImVec2 slot, ConnectionVCoords* conNode);
	void UpdateConnection(ImVec2 dif);
	void ResetConnection();
};



class VisualNode
{

public:


	//attributes 

	std::shared_ptr<Node> GNode; // the actual node that is linked to the visual part 
	GUIManager* Manager; //reduntant
	ImVec2 VNodePos;
	ImVec2 VNodeSize;
	
	std::vector<ConnectionVCoords> vInputs;
	std::vector<ConnectionVCoords> vOutputs;
	
	//methods

	void DrawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int steps);
	//constructors
	VisualNode(std::shared_ptr<Node> Gnode, ImVec2 position);
	~VisualNode();

	void SaveConnection(ImVec2 Coords,int slot,bool array);//nvm this for now
	void UpdateAllConnections(ImVec2 PosDif); //update connection coordinates when we move node
	void DisplayNode(ImDrawList* drawList,ImVec2 offset);
};

