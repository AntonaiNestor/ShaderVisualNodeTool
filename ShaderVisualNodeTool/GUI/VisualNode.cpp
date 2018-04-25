#include "VisualNode.h"



VisualNode::VisualNode(GUIManager* manager ,Node* Gnode, ImVec2 position )
{/*
	switch (type){
		case NodeType::OutputNode:
					
		break;


		default:

			break;
	}*/
	Manager = manager;
	VNodePos = position;
	VNodeSize = ImVec2(200, 75);
	GNode = Gnode;

}


VisualNode::~VisualNode()
{
}

void VisualNode::DisplayNode(ImDrawList * drawList,ImVec2 offset)
{
	//var initialisations
	ImVec2 NodeViewPos = Manager->NodeViewPos;
	ImVec2 NodeRelevantPos = NodeViewPos + VNodePos + offset;
	ImVec2 TitlePadding(TITLE_PADDING_X,TITLE_PADDING_Y);
	std::string NodeUniqueNameID = "Node " + std::to_string(GNode->UniqueID);


	drawList->ChannelsSetCurrent(0); // Background

	ImGui::SetCursorScreenPos(VNodePos + NodeViewPos);

	//Moveable node by title 
	
	//THIS ISN;T CORRECT I FEEL - MOVABLE SHOULD NOT BE INSIDE  DISPLAY BUT OUTSIDE AND JUST SEND THE OFFSET
	ImGui::InvisibleButton(NodeUniqueNameID.c_str(), ImVec2(VNodeSize.x, TITLE_BOX_HEIGHT));
	if (ImGui::IsItemHovered())
	{

	}
	bool node_moving_active = ImGui::IsItemActive();

	if (node_moving_active && ImGui::IsMouseDragging(0))
		VNodePos = VNodePos + ImGui::GetIO().MouseDelta;


	//this values need to be relevant to the zooming, for now everything will be fixed
	drawList->AddRectFilled(NodeRelevantPos, NodeRelevantPos + VNodeSize, ImColor(40, 40, 40), 10.0); //Background rect
	drawList->AddRectFilled(NodeRelevantPos, NodeRelevantPos + ImVec2(VNodeSize.x, TITLE_BOX_HEIGHT), ImColor(150, 50, 150), 10, ImDrawCornerFlags_Top); //Title rect 
	drawList->AddRect(NodeRelevantPos, NodeRelevantPos + VNodeSize, ImColor(255, 255, 255), 10.0); // Border
	drawList->AddLine(NodeRelevantPos + ImVec2(VNodeSize.x * 3 / 4, TITLE_BOX_HEIGHT), VNodePos + NodeViewPos + ImVec2(VNodeSize.x * 3 / 4, VNodeSize.y), ImColor(255, 255, 255)); // Output Divine Line


	//Title text
	ImGui::SetCursorScreenPos(NodeRelevantPos + TitlePadding);
	ImGui::Text("Constant :  ID - %d", GNode->UniqueID);

	//Main Body
	//ImGui::Spacing();
	ImGui::SetCursorScreenPos(NodeRelevantPos + ImVec2(PADDING_X, TITLE_BOX_HEIGHT + PADDING_Y));
	ImGui::BeginGroup(); // Lock horizontal position
	ImGui::Text("Float :");
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputFloat("",&(GNode->Output[0].Value), 0.0f, 0.0f, 3, 0);

	drawList->AddCircleFilled(ImVec2(NodeRelevantPos.x + VNodeSize.x, NodeRelevantPos.y + VNodeSize.y*0.6), 5, ImColor(255, 255, 255), 12);
	ImGui::EndGroup();



}
