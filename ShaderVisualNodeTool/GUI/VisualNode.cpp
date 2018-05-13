#include "VisualNode.h"
#include <iostream>


//struct implementations
ConnectionVCoords::ConnectionVCoords()
{
	SlotCoords = ImVec2(); conn = nullptr; connected = false; drawn = false;
}

void ConnectionVCoords::SetCoords(ImVec2 slot,ConnectionVCoords* conNode)
{
	SlotCoords = slot;
	conn = conNode;
	connected = true;
}

void ConnectionVCoords::UpdateConnection(ImVec2 dif) {
	SlotCoords = SlotCoords + dif; 
}

void ConnectionVCoords::ResetConnection() {

	SlotCoords = ImVec2(); conn = nullptr; connected = false; drawn = false;
}


void VisualNode::DrawHermite(ImDrawList * drawList, ImVec2 p1, ImVec2 p2, int steps)
{
	ImVec2 t1 = ImVec2(+80.0f, 0.0f);
	ImVec2 t2 = ImVec2(+80.0f, 0.0f);

	for (int step = 0; step <= steps; step++)
	{
		float t = (float)step / (float)steps;
		float h1 = +2 * t*t*t - 3 * t*t + 1.0f;
		float h2 = -2 * t*t*t + 3 * t*t;
		float h3 = t * t*t - 2 * t*t + t;
		float h4 = t * t*t - t * t;
		drawList->PathLineTo(ImVec2(h1*p1.x + h2 * p2.x + h3 * t1.x + h4 * t2.x, h1*p1.y + h2 * p2.y + h3 * t1.y + h4 * t2.y));
	}

	drawList->PathStroke(ImColor(200, 200, 100), false, 3.0f);

}

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
	//PosDif = ImVec2(0, 0);

	//populate coordinates with zero values
	for (auto it : GNode->Input) {
		vInputs.push_back(ConnectionVCoords());
	}

	for (auto it : GNode->Output) {
		vOutputs.push_back(ConnectionVCoords());
	}


}


VisualNode::~VisualNode()
{
	std::cout << " Destroying Visual Node " << std::endl;
}

void VisualNode::UpdateAllConnections(ImVec2 PosDif)
{
	for (auto &it : vInputs) { if (it.connected) { it.UpdateConnection(PosDif); } }
	for (auto &it : vOutputs) {
		if (it.connected) {
			//std::cout << it.SlotCoords.x << "  " << it.SlotCoords.y << std::endl;
			it.UpdateConnection(PosDif);
			//it.SlotCoords = it.SlotCoords + PosDif;
			//std::cout << it.SlotCoords.x << " After  " << it.SlotCoords.y << std::endl;

		}
	}
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

	if (node_moving_active && ImGui::IsMouseDragging(0)) {

		ImVec2 PosDif = ImGui::GetIO().MouseDelta;

		VNodePos = VNodePos + PosDif;
		//update all connections when you move node instantly
		UpdateAllConnections(PosDif);

	}
		
	//this values need to be relevant to the zooming, for now everything will be fixed
	drawList->AddRectFilled(NodeRelevantPos, NodeRelevantPos + VNodeSize, ImColor(40, 40, 40), 10.0); //Background rect
	drawList->AddRectFilled(NodeRelevantPos, NodeRelevantPos + ImVec2(VNodeSize.x, TITLE_BOX_HEIGHT), ImColor(150, 50, 150), 10, ImDrawCornerFlags_Top); //Title rect 
	drawList->AddRect(NodeRelevantPos, NodeRelevantPos + VNodeSize, ImColor(255, 255, 255), 10.0); // Border
	drawList->AddLine(NodeRelevantPos + ImVec2(VNodeSize.x * 3 / 4, TITLE_BOX_HEIGHT), VNodePos + NodeViewPos + ImVec2(VNodeSize.x * 3 / 4, VNodeSize.y), ImColor(255, 255, 255)); // Output Divine Line


	//Title text
	ImGui::SetCursorScreenPos(NodeRelevantPos + TitlePadding);
	ImGui::Text("Constant :  ID - %d", GNode->UniqueID);

	//Main Body

	//TODO this shouldn't be a float necessarily
	//ImGui::Spacing();
	ImGui::SetCursorScreenPos(NodeRelevantPos + ImVec2(PADDING_X, TITLE_BOX_HEIGHT + PADDING_Y));
	ImGui::BeginGroup(); // Lock horizontal position
	ImGui::Text("Float :");
	ImGui::SameLine();
	ImGui::PushItemWidth(50);
	ImGui::InputFloat("",&(GNode->Output[0].Value), 0.0f, 0.0f, 3, 0);

	//position of slot and button 

	//x = starting , y = variant

	float InputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vInputs.size()+1);
	float OutputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vOutputs.size() + 1);

	
	//Display all Input buttons and circles 
	for (int i = 0; i < vInputs.size(); i++) {
		ImVec2 InputPos(ImVec2(NodeRelevantPos.x, NodeRelevantPos.y + TITLE_BOX_HEIGHT + InputMargin * (i+1) ));
		//drawList->AddCircleFilled(InputPos, 5, ImColor(255, 255, 255), 12);

		//add the Input invisible button on top of the slot
		ImGui::SetCursorScreenPos(ImVec2(InputPos.x - 10, InputPos.y - 10));
		auto ButtonName = NodeUniqueNameID.append(" Input " + std::to_string(i)).c_str();
		
		if (ImGui::Button(ButtonName, ImVec2(20.0f, 20.0f))) {
			//std::cout << "Button Clicked" << std::endl;
		}
		if (ImGui::IsItemActive() && !(Manager->IsDrawing)) {

			//start drawing from this slot 

			
			Manager->IsDrawing = true;
			Manager->InitDrawingPos = InputPos;
			Manager->StartSlotType = Input;
			Manager->StartNode = this;
			Manager->StartIndex = i;

			std::cout << "Started drawing from " << Manager->StartSlotType << " slot : " << std::to_string(Manager->StartIndex) << std::endl;
		}

		if (ImGui::IsItemHovered() && (Manager->IsDrawing)) {
			//end drawing to this slot
			std::cout << "INPUT IS HOVERED!!!!!!" << std::endl;
			Manager->EndDrawingPos = InputPos;
			Manager->EndNode = this;
			Manager->EndSlotType = Input;
			Manager->EndIndex = i;
			Manager->ItemsHovered = true;
			//std::cout << "Ending drawing from " << Manager->EndSlotType << " slot : " << std::to_string(Manager->EndIndex) << std::endl;
		}

	}
	

	//Display all Output buttons and circles 
	for (int i = 0; i < vOutputs.size(); i++) {

		ImVec2 OutputPos(ImVec2(NodeRelevantPos.x + VNodeSize.x, NodeRelevantPos.y + TITLE_BOX_HEIGHT + OutputMargin * (i + 1)));
		drawList->AddCircleFilled(OutputPos, 5, ImColor(255, 255, 255), 12);

		//add the OutPut invisible button on top of the slot 
		ImGui::SetCursorScreenPos(ImVec2(OutputPos.x - 10.0f, OutputPos.y - 10));
		auto ButtonName = NodeUniqueNameID.append(" Output " + std::to_string(i)).c_str();

		if (ImGui::InvisibleButton(ButtonName, ImVec2(20.0f, 20.0f))) {
			//std::cout << "Button Clicked" << std::endl;
			
		}
		if (ImGui::IsItemActive() && !(Manager->IsDrawing)) {

			//start drawing from this slot 
			Manager->IsDrawing = true;
			Manager->InitDrawingPos = OutputPos;
			Manager->StartSlotType = Output;
			Manager->StartNode = this;
			Manager->StartIndex = i;
		}

		if (ImGui::IsItemHovered() && (Manager->IsDrawing)) {


			//Manager->IsDrawing = true;
			Manager->EndDrawingPos = OutputPos;
			//Manager->DrawPermanent = true;
			Manager->EndSlotType = Output;
			Manager->EndIndex = i;
			Manager->EndNode = this;
			Manager->ItemsHovered = true;
		}

	}



	ImGui::EndGroup();


	//--------Drawing connections 

	//Output
	for (int i = 0; i < vOutputs.size(); i++) {
		//if there is a connection in the graph then draw a line
		//but i could just check the visual node contents as well. DOUBLE INFORMATION ANTONY
		if (vOutputs.at(i).connected ) {
			DrawHermite(drawList,vOutputs.at(i).SlotCoords, vOutputs.at(i).conn->SlotCoords,20);
		}
	}

	//Input - Currently not used

	//for (int i = 0; i <vInputs.size(); i++) {
	//	//if there is a connection in the graph then draw a line
	//	if (vInputs.at(i).connected) {
	//		DrawHermite(drawList, vInputs.at(i).SlotCoords, vInputs.at(i).conn->SlotCoords, 20);
	//	}
	//}

}
