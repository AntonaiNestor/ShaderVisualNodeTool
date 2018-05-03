#include "VisualNode.h"
#include <iostream>


VisualNode::VisualNode(std::shared_ptr<Node> Gnode, ImVec2 position )
{/*
	switch (type){
		case NodeType::OutputNode:
					
		break;


		default:

			break;
	}*/

	Manager = GUIManager::getInstance();
	VNodePos = position;
	//THE SIZE SHOULD BE VARIABLE OR AT LEAST TAKEN FROM A LOOKUP TABLE
	VNodeSize = ImVec2(200, 75);
	GNode = Gnode;

}


VisualNode::~VisualNode()
{
	std::cout << " Destroying Visual Node " << std::endl;
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

	//position of slot and button

	ImVec2 InputPos(ImVec2(NodeRelevantPos.x , NodeRelevantPos.y + VNodeSize.y*0.6));
	drawList->AddCircleFilled(InputPos, 5, ImColor(255, 255, 255), 12);

	ImVec2 OutputPos(ImVec2(NodeRelevantPos.x + VNodeSize.x, NodeRelevantPos.y + VNodeSize.y*0.6));
	drawList->AddCircleFilled(OutputPos, 5, ImColor(255, 255, 255), 12);

	ImGui::EndGroup();

	//add the Input invisible button on top of the slot
	ImGui::SetCursorScreenPos(ImVec2(InputPos.x -10, InputPos.y - 10));
	//NodeUniqueNameID.append("Button").c_str()
	if (ImGui::InvisibleButton("In", ImVec2(20.0f, 20.0f))) {
		//std::cout << "Button Clicked" << std::endl;
		//drawingLine = true;

	}
	if (ImGui::IsItemActive() && !(Manager->IsDrawing)) {

		//start drawing from this slot 
		Manager->IsDrawing = true;
		Manager->InitDrawingPos = InputPos;
		Manager->StartSlotType = Input;
		Manager->StartIndex = 0;
	}

	if (ImGui::IsItemHovered() && (Manager->IsDrawing)) {


		//Manager->IsDrawing = true;
		Manager->EndDrawingPos = InputPos;
		//Manager->DrawPermanent = true;
		Manager->EndSlotType = Input;
		Manager->EndIndex = 0;
	}




	//add the OutPut invisible button on top of the slot 
	ImGui::SetCursorScreenPos(ImVec2(OutputPos.x - 10.0f, OutputPos.y - 10));
	//NodeUniqueNameID.append("Button").c_str()
	if (ImGui::InvisibleButton("Out",ImVec2(20.0f,20.0f))) {
			//std::cout << "Button Clicked" << std::endl;
			//drawingLine = true;
			
	}
	if (ImGui::IsItemActive() && !(Manager->IsDrawing)) {
		
		

		//start drawing from this slot 
		Manager->IsDrawing = true;
		Manager->InitDrawingPos = OutputPos;
		Manager->StartSlotType = Output;
		Manager->StartIndex = 0;
	}

	if (ImGui::IsItemHovered() && (Manager->IsDrawing)) {

		
		//Manager->IsDrawing = true;
		Manager->EndDrawingPos = OutputPos;
		Manager->DrawPermanent = true;
		Manager->EndSlotType = Output;
		Manager->EndIndex = 0;
	}


}
