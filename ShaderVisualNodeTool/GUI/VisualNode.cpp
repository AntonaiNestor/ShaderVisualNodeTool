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
{

	Manager = GUIManager::getInstance();
	VNodePos = position;
	//THE SIZE SHOULD BE VARIABLE OR AT LEAST TAKEN FROM A LOOKUP TABLE

	
	GNode = Gnode;
	//PosDif = ImVec2(0, 0);

	//populate coordinates with zero values
	for (auto it : GNode->Input) {
		vInputs.push_back(ConnectionVCoords());
	}

	for (auto it : GNode->Output) {
		vOutputs.push_back(ConnectionVCoords());
	}
	if (GNode->Type == OutputNode) {

		VNodeSize = ImVec2(200, 200);
	}

	else {
		VNodeSize = ImVec2(200, 75);
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

	bool NodeDrawn = true;
	//DrawConstantNode(drawList, offset);
	switch (GNode->Type)
	{
		case (InputNode):
			DrawInputNode(drawList, offset);

			break;
		case(FunctionNode):
			DrawFunctionNode( drawList, offset);
			break;

		case(OutputNode):
			DrawOutputNode( drawList,offset);
			break;

		default:
			NodeDrawn = false;
			//draw nothing
			break;
	}

	//--------Drawing connections 

	if (NodeDrawn) {
	
		//Output
		for (int i = 0; i < vOutputs.size(); i++) {
			//if there is a connection in the graph then draw a line
			//but i could just check the visual node contents as well. DOUBLE INFORMATION ANTONY
			if (vOutputs.at(i).connected) {
				DrawHermite(drawList, vOutputs.at(i).SlotCoords, vOutputs.at(i).conn->SlotCoords, 20);
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
	

}

void VisualNode::DrawInputNode(ImDrawList * drawList, ImVec2 offset)
{
	//var initialisations

	ImVec2 NodeViewPos = Manager->NodeViewPos;
	ImVec2 NodeRelevantPos = NodeViewPos + VNodePos + offset;
	ImVec2 TitlePadding(TITLE_PADDING_X, TITLE_PADDING_Y);
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
	std::string NodeName = GNode->Name;
	auto TitleText = (NodeName.append(" : ID - ")).append(std::to_string(GNode->UniqueID)).c_str();
	ImGui::Text(TitleText);

	//Main Body


	float InputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vInputs.size() + 1);
	float OutputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vOutputs.size() + 1);

	//
	////Display all Input buttons and circles 


	//for (int i = 0; i < vInputs.size(); i++) {



	//	ImVec2 InputPos(ImVec2(NodeRelevantPos.x, NodeRelevantPos.y + TITLE_BOX_HEIGHT + InputMargin * (i + 1)));
	//	drawList->AddCircleFilled(InputPos, 5, ImColor(255, 255, 255), 12);

	//	//add the Input invisible button on top of the slot
	//	ImGui::SetCursorScreenPos(ImVec2(InputPos.x - 10, InputPos.y - 10));
	//	auto ButtonName = NodeUniqueNameID.append(" Input " + std::to_string(i)).c_str();

	//	if (ImGui::InvisibleButton(ButtonName, ImVec2(20.0f, 20.0f))) {
	//		//std::cout << "Button Clicked" << std::endl;
	//	}
	//	if (ImGui::IsItemActive() && !(Manager->IsDrawing)) {

	//		//start drawing from this slot 

	//		Manager->IsDrawing = true;
	//		Manager->InitDrawingPos = InputPos;
	//		Manager->StartSlotType = Input;
	//		Manager->StartNode = this;
	//		Manager->StartIndex = i;

	//		//std::cout << "Started drawing from " << Manager->StartSlotType << " slot : " << std::to_string(Manager->StartIndex) << std::endl;
	//	}

	//	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && Manager->IsDrawing) {
	//		//end drawing to this slot

	//		Manager->EndDrawingPos = InputPos;
	//		Manager->EndNode = this;
	//		Manager->EndSlotType = Input;
	//		Manager->EndIndex = i;
	//		Manager->ItemsHovered = true;
	//		//std::cout << "Ending drawing from " << Manager->EndSlotType << " slot : " << std::to_string(Manager->EndIndex) << std::endl;
	//	}


	//	ImVec2 InputValPos = ImVec2(InputPos.x + PADDING_X, InputPos.y - PADDING_Y);
	//	ImGui::SetCursorScreenPos(InputValPos);


	//	ImGui::BeginGroup(); // Lock horizontal position
	//						 //Input Var NameX	
	//	ImGui::Text("Float :");
	//	ImGui::SameLine();
	//	ImGui::PushItemWidth(50);
	//	//Input Current value -- This probably needs to be type 
	//	ImGui::InputFloat("", &(GNode->Input[i].Value), 0.0f, 1.0f, 3, 0);
	//	ImGui::EndGroup();


	//}


	//Display all Output buttons and circles 
	for (int i = 0; i < vOutputs.size(); i++) {

		ImVec2 OutputPos(ImVec2(NodeRelevantPos.x + VNodeSize.x, NodeRelevantPos.y + TITLE_BOX_HEIGHT + OutputMargin * (i + 1)));
		drawList->AddCircleFilled(OutputPos, 5, ImColor(255, 255, 255), 12);


		ImVec2 OutputValPos = ImVec2(NodeRelevantPos.x + PADDING_X, OutputPos.y- 0.25*OutputMargin);
		ImGui::SetCursorScreenPos(OutputValPos);


		ImGui::PushID(i+GNode->UniqueID);
		ImGui::BeginGroup(); // Lock horizontal position
							 //Input Var NameX	
		// TODO HERE this should be custom depending on type of costant node type --------------
		ImGui::Text("Float :");
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		//Input Current value -- This probably needs to be type 
		auto ValName =  std::to_string(GNode->UniqueID);
		//ValName.c_str()
		ImGui::InputFloat("", &(GNode->Output[i].Value), 0.0f, 1.0f, 3, 0);
		ImGui::EndGroup();
		

		//Output name 
		ImVec2 OutputTextPos = ImVec2(OutputPos.x - 1.5*PADDING_X, OutputPos.y - PADDING_Y);
		ImGui::SetCursorScreenPos(OutputTextPos);
		//Output Var Name + type prolly 	

		//auto name = Manager->GetSlotValueName("Out", Float);
		ImGui::Text("Out");

		ImGui::PopID();

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

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && Manager->IsDrawing) {


			//Manager->IsDrawing = true;
			Manager->EndDrawingPos = OutputPos;
			//Manager->DrawPermanent = true;
			Manager->EndSlotType = Output;
			Manager->EndIndex = i;
			Manager->EndNode = this;
			Manager->ItemsHovered = true;
		}

	}

}

void VisualNode::DrawFunctionNode(ImDrawList * drawList, ImVec2 offset)
{
	//var initialisations

	ImVec2 NodeViewPos = Manager->NodeViewPos;
	ImVec2 NodeRelevantPos = NodeViewPos + VNodePos + offset;
	ImVec2 TitlePadding(TITLE_PADDING_X, TITLE_PADDING_Y);
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

	//these values need to be relevant to the zooming, for now everything will be fixed
	drawList->AddRectFilled(NodeRelevantPos, NodeRelevantPos + VNodeSize, ImColor(40, 40, 40), 10.0); //Background rect
	drawList->AddRectFilled(NodeRelevantPos, NodeRelevantPos + ImVec2(VNodeSize.x, TITLE_BOX_HEIGHT), ImColor(150, 50, 150), 10, ImDrawCornerFlags_Top); //Title rect 
	drawList->AddRect(NodeRelevantPos, NodeRelevantPos + VNodeSize, ImColor(255, 255, 255), 10.0); // Border
	drawList->AddLine(NodeRelevantPos + ImVec2(VNodeSize.x * 3 / 4, TITLE_BOX_HEIGHT), VNodePos + NodeViewPos + ImVec2(VNodeSize.x * 3 / 4, VNodeSize.y), ImColor(255, 255, 255)); // Output Divine Line


																																												   //Title text
	ImGui::SetCursorScreenPos(NodeRelevantPos + TitlePadding);
	std::string NodeName = GNode->Name;
	auto TitleText = (NodeName.append(" : ID - ")).append(std::to_string(GNode->UniqueID)).c_str();
	ImGui::Text(TitleText);

	//Main Body


	float InputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vInputs.size() + 1);
	float OutputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vOutputs.size() + 1);

	//
	////Display all Input buttons and circles 

	ImGui::PushID(this);
	for (int i = 0; i < vInputs.size(); i++) {


		
		ImVec2 InputPos(ImVec2(NodeRelevantPos.x, NodeRelevantPos.y + TITLE_BOX_HEIGHT + InputMargin * (i + 1)));
		drawList->AddCircleFilled(InputPos, 5, ImColor(255, 255, 255), 12);

		//add the Input invisible button on top of the slot
		ImGui::SetCursorScreenPos(ImVec2(InputPos.x - 10, InputPos.y - 10));
		auto ButtonName = NodeUniqueNameID.append(" Input " + std::to_string(i)).c_str();

		if (ImGui::InvisibleButton(ButtonName, ImVec2(20.0f, 20.0f))) {
			//std::cout << "Button Clicked" << std::endl;
		}
		if (ImGui::IsItemActive() && !(Manager->IsDrawing)) {

			//start drawing from this slot 

			Manager->IsDrawing = true;
			Manager->InitDrawingPos = InputPos;
			Manager->StartSlotType = Input;
			Manager->StartNode = this;
			Manager->StartIndex = i;

			//std::cout << "Started drawing from " << Manager->StartSlotType << " slot : " << std::to_string(Manager->StartIndex) << std::endl;
		}

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && Manager->IsDrawing) {
			//end drawing to this slot

			Manager->EndDrawingPos = InputPos;
			Manager->EndNode = this;
			Manager->EndSlotType = Input;
			Manager->EndIndex = i;
			Manager->ItemsHovered = true;
			//std::cout << "Ending drawing from " << Manager->EndSlotType << " slot : " << std::to_string(Manager->EndIndex) << std::endl;
		}


		ImVec2 InputValPos = ImVec2(InputPos.x + PADDING_X, InputPos.y - PADDING_Y);
		ImGui::SetCursorScreenPos(InputValPos);
		ImGui::PushID(i);
		
		ImGui::BeginGroup(); // Lock horizontal position
							 //Input Var NameX	
		ImGui::Text((GNode->Input.at(i).Name).c_str());
		ImGui::SameLine();
		ImGui::PushItemWidth(50);
		//Input Current value -- This probably needs to be type 
		ImGui::InputFloat("", &(GNode->Input[i].Value), 0.0f, 1.0f, 3, 0);
		ImGui::EndGroup();

		ImGui::PopID();
		

	}
	ImGui::PopID();

	//Display all Output buttons and circles 
	for (int i = 0; i < vOutputs.size(); i++) {

		ImVec2 OutputPos(ImVec2(NodeRelevantPos.x + VNodeSize.x, NodeRelevantPos.y + TITLE_BOX_HEIGHT + OutputMargin * (i + 1)));
		drawList->AddCircleFilled(OutputPos, 5, ImColor(255, 255, 255), 12);


		//Output name 
		ImVec2 OutputTextPos = ImVec2(OutputPos.x - 1.5 * PADDING_X, OutputPos.y - PADDING_Y);
		ImGui::SetCursorScreenPos(OutputTextPos);
		//Output Var Name + type prolly 	
		ImGui::Text("Out");

		

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

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && Manager->IsDrawing) {


			//Manager->IsDrawing = true;
			Manager->EndDrawingPos = OutputPos;
			//Manager->DrawPermanent = true;
			Manager->EndSlotType = Output;
			Manager->EndIndex = i;
			Manager->EndNode = this;
			Manager->ItemsHovered = true;
		}

	}



}

void VisualNode::DrawOutputNode(ImDrawList * drawList, ImVec2 offset)
{

	//var initialisations

	ImVec2 NodeViewPos = Manager->NodeViewPos;
	ImVec2 NodeRelevantPos = NodeViewPos + VNodePos + offset;
	ImVec2 TitlePadding(TITLE_PADDING_X, TITLE_PADDING_Y);
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

	//these values need to be relevant to the zooming, for now everything will be fixed
	drawList->AddRectFilled(NodeRelevantPos, NodeRelevantPos + VNodeSize, ImColor(40, 40, 40), 10.0); //Background rect
	drawList->AddRectFilled(NodeRelevantPos, NodeRelevantPos + ImVec2(VNodeSize.x, TITLE_BOX_HEIGHT), ImColor(150, 50, 150), 10, ImDrawCornerFlags_Top); //Title rect 
	drawList->AddRect(NodeRelevantPos, NodeRelevantPos + VNodeSize, ImColor(255, 255, 255), 10.0); // Border
	//drawList->AddLine(NodeRelevantPos + ImVec2(VNodeSize.x * 3 / 4, TITLE_BOX_HEIGHT), VNodePos + NodeViewPos + ImVec2(VNodeSize.x * 3 / 4, VNodeSize.y), ImColor(255, 255, 255)); // Output Divine Line


																																												   //Title text
	ImGui::SetCursorScreenPos(NodeRelevantPos + TitlePadding);
	std::string NodeName = GNode->Name;
	auto TitleText = (NodeName.append(" : ID - ")).append(std::to_string(GNode->UniqueID)).c_str();
	ImGui::Text(TitleText);

	//Main Body


	float InputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vInputs.size() + 1);
	//float OutputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vOutputs.size() + 1);

	//
	////Display all Input buttons and circles 

	ImGui::PushID(this);
	for (int i = 0; i < vInputs.size(); i++) {


		ImVec2 InputPos(ImVec2(NodeRelevantPos.x, NodeRelevantPos.y + TITLE_BOX_HEIGHT + InputMargin * (i + 1)));
		drawList->AddCircleFilled(InputPos, 5, ImColor(255, 255, 255), 12);

		//add the Input invisible button on top of the slot
		ImGui::SetCursorScreenPos(ImVec2(InputPos.x - 10, InputPos.y - 10));
		auto ButtonName = NodeUniqueNameID.append(" Input " + std::to_string(i)).c_str();

		if (ImGui::InvisibleButton(ButtonName, ImVec2(20.0f, 20.0f))) {
			//std::cout << "Button Clicked" << std::endl;
		}
		if (ImGui::IsItemActive() && !(Manager->IsDrawing)) {

			//start drawing from this slot 

			Manager->IsDrawing = true;
			Manager->InitDrawingPos = InputPos;
			Manager->StartSlotType = Input;
			Manager->StartNode = this;
			Manager->StartIndex = i;

			//std::cout << "Started drawing from " << Manager->StartSlotType << " slot : " << std::to_string(Manager->StartIndex) << std::endl;
		}

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem) && Manager->IsDrawing) {
			//end drawing to this slot

			Manager->EndDrawingPos = InputPos;
			Manager->EndNode = this;
			Manager->EndSlotType = Input;
			Manager->EndIndex = i;
			Manager->ItemsHovered = true;
			//std::cout << "Ending drawing from " << Manager->EndSlotType << " slot : " << std::to_string(Manager->EndIndex) << std::endl;
		}


		ImVec2 InputValPos = ImVec2(InputPos.x + PADDING_X, InputPos.y - PADDING_Y);
		ImGui::SetCursorScreenPos(InputValPos);
		ImGui::PushID(i);

		//ImGui::BeginGroup(); // Lock horizontal position
							 //Input Var NameX	
		ImGui::Text((*(Graph::getInstance()->ShaderCode)).c_str());
		
		
		ImGui::PopID();


	}
	ImGui::PopID();

	
}
