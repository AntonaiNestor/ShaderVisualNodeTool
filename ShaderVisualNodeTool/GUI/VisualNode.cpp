#include "VisualNode.h"
#include <iostream>
#include <algorithm>
#include "../NodeSystem/InputNode.h"
#include "../NodeSystem/TimeNode.h"

//struct implementations
ConnectionVCoords::ConnectionVCoords()
{
	SlotCoords = ImVec2(); 
	//conn = nullptr; 
	//SlotCoords.push_back(ImVec2());
	//conn.push_back(nullptr);
	connected = false; drawn = false;
}

void ConnectionVCoords::SetCoords(ImVec2 slot,ConnectionVCoords* conNode)
{
	SlotCoords = slot;
	conn[0] = conNode;
	connected = true;
}

void ConnectionVCoords::AddCoords(ImVec2 slot, ConnectionVCoords * conNode)
{
	//adds a pair of coords and pointers to the current 
	//redundant?
	SlotCoords = slot;
	conn.push_back(conNode);
	connected = true;
}

void ConnectionVCoords::RemoveCoords(ConnectionVCoords * conNode)
{
	for (int i = 0; i < conn.size(); i++) {
		if (conn.at(i) == conNode) {
			conn.erase(conn.begin() + i) ;

			if (conn.empty()) connected = false;
			break; 
		}
	}
}

void ConnectionVCoords::UpdateConnection(ImVec2 dif) {

	
		SlotCoords = SlotCoords + dif;
	
	
}

void ConnectionVCoords::ResetConnection() {

	SlotCoords = ImVec2();  connected = false; drawn = false; conn.clear();
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

	int maxSlotSize = std::max(GNode->Input.size(),GNode->Output.size());
	VNodeSize = ImVec2(200, 50+ 25 * maxSlotSize);

		
	

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
		case(InputnodeT):
			DrawInputNode(drawList, offset);

			break;
		case(FunctionnodeT):
			DrawFunctionNode( drawList, offset);
			break;

		case(OutputnodeT):
			DrawOutputNode( drawList,offset);
			break;
		case(ShadernodeT):
			DrawOutputNode(drawList, offset);
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

			auto outCoords = vOutputs.at(i);
			for (int ConnIndex = 0; ConnIndex < outCoords.conn.size(); ConnIndex++) {
				//TODO check if connected makes sense here

				//INDICES PROBABLY WRONG HERE but maybe the 1-1 connection is fine
				DrawHermite(drawList, outCoords.SlotCoords, outCoords.conn[ConnIndex]->SlotCoords, 20);
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
	drawList->AddLine(NodeRelevantPos + ImVec2(VNodeSize.x * 3.3 / 4, TITLE_BOX_HEIGHT), VNodePos + NodeViewPos + ImVec2(VNodeSize.x * 3.3 / 4, VNodeSize.y), ImColor(255, 255, 255)); // Output Divine Line


																																												   //Title text
	ImGui::SetCursorScreenPos(NodeRelevantPos + TitlePadding);
	std::string NodeName = GNode->Name;
	auto TitleText = (NodeName.append(" : ID - ")).append(std::to_string(GNode->UniqueID)).c_str();
	ImGui::Text(TitleText);

	//Main Body


	float InputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vInputs.size() + 1);
	float OutputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vOutputs.size() + 1);

	//
	

	// General BeginCombo() API, you have full control over your selection data and display type.
	// (your selection data could be an index, a pointer to the object, an id for the object, a flag stored in the object itself, etc.)

	ImGui::PushID(GNode->UniqueID + NodeRelevantPos.x);

	auto graph = Graph::getInstance();
	bool displayAttribs = false;
	bool displayMat = false;
	const char* item_current = graph->VariableTypes[0];
	
	//ARGH STOP THIS, YOU ARE KILLING ME

	//if it is not the case of a Time node where we do not want to draw anything (for now)
	if (typeid(*GNode) != typeid(TimeNode)) {
	

		//check the current type of inputNode to either update the dropdown list or to draw 
		//specific contents for the Attributes node

		switch (dynamic_cast<InputNode&>(*GNode).inputType) {

		case (0):
			item_current = graph->VariableTypes[0];
			break;
		case (1):
			item_current = graph->VariableTypes[1];
			break;
		case(2):
			item_current = graph->VariableTypes[2];
			break;
		case(3): {
			displayAttribs = true;
			break;
		}
		case(4): {
			displayMat = true;
			break;
		}
		default:
			//item_current = graph->VariableTypes[0];
			break;

		}

		//if this is isn't an attribute node
		if (!displayAttribs && !displayMat){
			ImGui::PushItemWidth(80);
			ImGui::SetCursorScreenPos(ImVec2(NodeRelevantPos.x, NodeRelevantPos.y + 1 + TITLE_BOX_HEIGHT));
			if (ImGui::BeginCombo("T" + GNode->UniqueID, item_current, 1)) // The second parameter is the label previewed before opening the combo.
			{
			
				for (int n = 0; n < 2; n++)
				{
					bool is_selected = (GNode->Type == BaseNodeType(n));
					if (ImGui::Selectable(graph->VariableTypes[n], is_selected)) {
				
						dynamic_cast<InputNode&>(*GNode).inputType= InputNodeType(n);

						// EXTREMELY UGLY 
						if (n == 1) {
							//dynamic_cast<InputNode&>(*GNode).EditUniform();
							//std::shared_ptr<InputNode> unif = dynamic_cast<InputNode*>(GNode);

							//REMEMBER TO REMOVE FROM LIST WHEN GOING BACK TO CONSTANT
							graph->UniformNodes.push_back(GNode);

						}

						Manager->ValueChanged = true;

					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}
				ImGui::EndCombo();
			}
		}

		//if this is a transformation matrix node 
	    if (displayMat  ) {
			ImGui::PushItemWidth(80);
			ImGui::SetCursorScreenPos(ImVec2(NodeRelevantPos.x, NodeRelevantPos.y + 1 + TITLE_BOX_HEIGHT));

			//This needs to move to a central position
			std::string Matrices[4] = {"Model", "View", "Projection","MVP"};
			const char* MatricesCC[4] = { "Model", "View", "Projection","MVP" };
			const char*  curr_choice = (GNode->Output[0].Name).c_str() ;

			//Display a dropdown menu with the available matrices to choose from. For now it iwll only be Model,Projection,View,MVP
			if (ImGui::BeginCombo("a" + GNode->UniqueID, curr_choice, 1)) // The second parameter is the label previewed before opening the combo.
			{

				for (int n = 0; n < 4; n++)
				{
					bool is_selected = !(GNode->Output[0].Name.compare(Matrices[n]));
					if (ImGui::Selectable(MatricesCC[n], is_selected)) {

						//depending on your selection, choose different uniform name to use
						GNode->Output.at(0).Name = Matrices[n];

						Manager->ValueChanged = true;

						//std::cout << GNode->Output.at(0).Name << std::endl;
					}

					if (is_selected)
						ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
				}
				ImGui::EndCombo();
			}

		}


		
	
	}


	ImGui::PopID();
	//Display all Output buttons and circles 
	for (int i = 0; i < vOutputs.size(); i++) {

		ImVec2 OutputPos(ImVec2(NodeRelevantPos.x + VNodeSize.x, NodeRelevantPos.y + TITLE_BOX_HEIGHT + 10 +  OutputMargin * (i + 1)));
		drawList->AddCircleFilled(OutputPos, 5, ImColor(255, 255, 255), 12);



		ImGui::PushID(i+GNode->UniqueID);
		ImGui::BeginGroup(); // Lock horizontal position		
		
		//Input Current value -- This probably needs to be type 
		auto ValName =  std::to_string(GNode->UniqueID);
		ImVec2 OutputValPos;


		//Custom inputs fields depending on valuetype and if the input node type is not attributes
		//
		if (!displayAttribs) {
			switch (GNode->Output[i].VariableType) {
			case(Float): {
				ImGui::PushItemWidth(40);
				//MORE TIME NODE BULLSHIT
				if (typeid(*GNode) != typeid(TimeNode)){
					OutputValPos = ImVec2(NodeRelevantPos.x + PADDING_X+25, OutputPos.y - 0.25*OutputMargin);
					ImGui::SetCursorScreenPos(OutputValPos);
				
					if (ImGui::InputFloat("", &(GNode->value.f_var), 0.0f, 0.0f, 2)) {
						//if (GNode->Type == BaseNodeType::InputnodeUniform) { graph->UpdateUniforms(); }
						if (dynamic_cast<InputNode&>(*GNode).inputType == UniformVariable) { graph->UpdateUniforms(); }
						//dynamic_cast<InputNode&>(*GNode).EditUniform()
						else { Manager->ValueChanged = true; }
					}
			
				

				}
				break;
			}
			
			case(Int): {
				OutputValPos = ImVec2(NodeRelevantPos.x + PADDING_X + 25, OutputPos.y - 0.25*OutputMargin);
				ImGui::SetCursorScreenPos(OutputValPos);
				ImGui::PushItemWidth(40);
			
				if (ImGui::InputInt("", &(GNode->value.i_var), 0.0f, 0.0f, 0)) {
					if (dynamic_cast<InputNode&>(*GNode).inputType == UniformVariable) { graph->UpdateUniforms(); }
					else { Manager->ValueChanged = true; }
				}
			
				break;
			}
			case(Vec2): {
				OutputValPos = ImVec2(NodeRelevantPos.x + PADDING_X + 15, OutputPos.y - 0.25*OutputMargin);
				ImGui::SetCursorScreenPos(OutputValPos);
			
				ImGui::PushItemWidth(80);
			
				if (ImGui::InputFloat2("", &(GNode->value.vec2_var.x), 2)) {
					if (dynamic_cast<InputNode&>(*GNode).inputType == UniformVariable) { graph->UpdateUniforms(); }
					else { Manager->ValueChanged = true; }
				}
			
				break;
			}
			case(Vec3): {
				OutputValPos = ImVec2(NodeRelevantPos.x + PADDING_X , OutputPos.y - 0.25*OutputMargin);
				ImGui::SetCursorScreenPos(OutputValPos);
				ImGui::PushItemWidth(120);
			
				if (ImGui::InputFloat3("", &(GNode->value.vec3_var.x), 2)) {
					if (dynamic_cast<InputNode&>(*GNode).inputType == UniformVariable) { graph->UpdateUniforms(); }
					else { Manager->ValueChanged = true; }
				}
			
				break;
			}
			case(Vec4): {
				OutputValPos = ImVec2(NodeRelevantPos.x + 10 , OutputPos.y - 0.25*OutputMargin);
				ImGui::SetCursorScreenPos(OutputValPos);
				ImGui::PushItemWidth(150);
			
				if (ImGui::InputFloat4("", &(GNode->value.vec4_var.x), 2)) {
					if (dynamic_cast<InputNode&>(*GNode).inputType == UniformVariable) {
						graph->UpdateUniforms(); 
					}
					else { Manager->ValueChanged = true; }
				
				}
				break;
			}

			default :
				break;
		
			}
			
		}
		else {
			OutputValPos = ImVec2(NodeRelevantPos.x + 50, OutputPos.y);
			ImGui::SetCursorScreenPos(OutputValPos);
			ImGui::PushItemWidth(50);
			ImGui::Text((GNode->Output[i].Name).c_str());
	
		}
		ImGui::PopItemWidth();
		ImGui::EndGroup();

		//Output name 
		ImVec2 OutputTextPos = ImVec2(OutputPos.x - 1.5* PADDING_X, OutputPos.y - PADDING_Y);
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
	ImVec2 TitlePadding(10, TITLE_PADDING_Y);
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
	auto TitleText = (NodeName.append(": ID - ")).append(std::to_string(GNode->UniqueID)).c_str();
	ImGui::Text(TitleText);

	//Main Body


	float InputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vInputs.size() + 1);
	float OutputMargin = ((NodeRelevantPos.y + VNodeSize.y) - (NodeRelevantPos.y + TITLE_BOX_HEIGHT)) / (vOutputs.size() + 1);

	//

	// Dropdown menu for selecting which shader the code will be executed in

	ImGui::PushID(GNode->UniqueID + NodeRelevantPos.x);
	const char* item_current;
	auto graph = Graph::getInstance();

	item_current = graph->ShaderTypes[GNode->CurrShaderType];

	ImGui::PushItemWidth(100);
	ImGui::SetCursorScreenPos(ImVec2(NodeRelevantPos.x, NodeRelevantPos.y + 1 + TITLE_BOX_HEIGHT));
	if (ImGui::BeginCombo("" + GNode->UniqueID, item_current, 1)) // The second parameter is the label previewed before opening the combo.
	{
		for (int n = 0; n < GNode->AllowedExecShaderTypes.size(); n++)
		{
			
			bool is_selected = (GNode->CurrShaderType == GNode->AllowedExecShaderTypes[n]);
			if (ImGui::Selectable(graph->ShaderTypes[GNode->AllowedExecShaderTypes[n]], is_selected)) {

				//dynamic_cast<InputNode&>(*GNode).inputType = InputNodeType(n);
				GNode->CurrShaderType = GNode->AllowedExecShaderTypes[n];
				std::cout << GNode->CurrShaderType << std::endl;
				Manager->ValueChanged = true;
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();


	////Display all Input buttons and circles 

	ImGui::PushID(this);
	for (int i = 0; i < vInputs.size(); i++) {


		
		ImVec2 InputPos(ImVec2(NodeRelevantPos.x, NodeRelevantPos.y + TITLE_BOX_HEIGHT + InputMargin * (i + 1)));
		drawList->AddCircleFilled(InputPos, 5, ImColor(255, 255, 255), 12);

		//TESTING THE BUTTONS
		
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
		

		if (GNode->Input[i].VariableType == Bool) {
			
			if (ImGui::Checkbox((GNode->Input.at(i).Name).c_str(), &(GNode->Input.at(i).Value.b_var))) {
				Manager->ValueChanged = true;
			}
			//
			
		}
		else {
		
			ImGui::Text((GNode->Input.at(i).Name).c_str());
		}
		//ImGui::SameLine();
		//ImGui::PushItemWidth(50);
		////Input Current value -- This probably needs to be type 
		//if (ImGui::InputFloat("", &(GNode->Input[i].Value), 0.0f, 1.0f, 3, 0)) {
		//	Manager->ValueChanged = true;
		//}
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

		ImGui::BeginGroup(); // Lock horizontal position
							 //Input Var NameX	
		std::string slotname = GNode->Input.at(i).Name;
		slotname.erase(std::remove(slotname.begin(), slotname.end(), '$'), slotname.end());
		ImGui::Text(slotname.c_str() );
		ImGui::SameLine();
		//ImGui::PushItemWidth(50);
		//Input Current value -- This probably needs to be type 
		/*if (ImGui::InputFloat("", &(GNode->Input[i].Value), 0.0f, 1.0f, 3, 0)) {
			Manager->ValueChanged = true;
		}*/
		ImGui::EndGroup();
		ImGui::PopID();
		


	}
	ImGui::PopID();


}
	

	

