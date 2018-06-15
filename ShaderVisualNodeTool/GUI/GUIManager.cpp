#include "GUIManager.h"
#include "../NodeSystem/InputNode.h"

#include "../NodeSystem/OutputNode.h"
#include "../NodeSystem/FunctionNode.h"
#include "../NodeSystem/TimeNode.h"
#include <iostream>






//null since it will be created on demand
GUIManager* GUIManager::Instance = 0;

GUIManager *GUIManager::getInstance()
{
	if (Instance == 0)
	{
		Instance = new GUIManager();
	}

	return Instance;
}

GUIManager::GUIManager()
{
	Context = ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	StartSlotType = EndSlotType = NodeSlotType::DefaultNodeSlotType;
}
//
//
//GUIManager::~GUIManager()
//{
//}


void GUIManager::CreateNode(ImVec2 pos, BaseNodeType type , std::string Name)
{
	// 1) Create Graph Node   
	// Creation of the object in the heap, using make shared. The pointer is local in the stack but 
	// since we push the object in the graph's list then a reference is kept.
	
	std::shared_ptr<Node> newGraphNode;


	
	switch (type) {
	case(InputnodeT):{
		//UGLY AGAIN , WHEN WILL THIS STOP
		if (Name.compare("time") == 0) {
			newGraphNode = std::make_shared<TimeNode>();
			
		}

		else {
			auto nodeInfo = Graph::getInstance()->InputNodes[Name];
			newGraphNode = std::make_shared<InputNode>(nodeInfo);
		}
	
		break;
	}
	case (OutputnodeT):{
		//std::string OutCode = "FragColor = vec4 ($r , $g, $b, $a );"; 
		//std::string OutCode = "FragColor = $color;";
		auto nodeInfo = Graph::getInstance()->ShaderNodes[Name];
		newGraphNode = std::make_shared<class::OutputNode>(nodeInfo);
		break;
	}
	case (FunctionnodeT):{
		
		auto nodeInfo = Graph::getInstance()->FunctionNodes[Name];

		newGraphNode = std::make_shared<class::FunctionNode>(nodeInfo);
		break;
	}
	default :
		break;
	}


	
	Graph::getInstance()->AddNode(newGraphNode);
	// 2) Create Visual Node at pos - same logic as above, object is in the heap but pointer is in the stack

	auto NewVnode= std::make_shared<VisualNode>(newGraphNode,pos);
	VNodeList.push_back(NewVnode);
	

}

void GUIManager::RenderDrawing(ImDrawList* drawlist)
{

	
	//temporary drawing of a one way line
   	if (IsDrawing) {
		DrawHermite(drawlist, InitDrawingPos, ImGui::GetMousePos(), 20);
    }
	
	//Mouse release will stp drawing temporary line and will decide if a permanent connection needs to be added to the list 
	 if (ImGui::IsMouseReleased(0) && IsDrawing) {
		
		 IsDrawing = false;
		 //if valid new connection --
		 // input -> output
		 // output -> input
		 //
			
		 if ( ( (StartSlotType == Input && EndSlotType == Output) ||
			 (StartSlotType == Output && EndSlotType == Input) ) && 
			 StartNode->GNode->UniqueID != EndNode->GNode->UniqueID 
			) {
			
			 //update the 2 nodes 
			 //both VNode and gNode connections

			 //REPLACE THE CONNECTION HERE LIKE YOU DO WITH THE GRAPH
			 if (StartSlotType == Input && 
				 StartNode->GNode->Input.at(StartIndex).VariableType == EndNode->GNode->Output.at(EndIndex).VariableType) {

				 //visual connection

				 //first remove whatever connection existed already in both old slots
				 
				 if (StartNode->vInputs.at(StartIndex).conn != nullptr) {
					 StartNode->vInputs.at(StartIndex).conn->ResetConnection();
				 }
				 
				 if (EndNode->vOutputs.at(EndIndex).conn != nullptr) {
					 EndNode->vOutputs.at(EndIndex).conn->ResetConnection();
				 }
				 //and then put the new connection

				 StartNode->vInputs.at(StartIndex).SetCoords(InitDrawingPos,&EndNode->vOutputs.at(EndIndex));
				 EndNode->vOutputs.at(EndIndex).SetCoords(EndDrawingPos,&StartNode->vInputs.at(StartIndex));


				 //Graph Node
				 Graph::getInstance()->CreateConnectionInOut(StartNode->GNode->shared_from_this(),EndNode->GNode,StartIndex,EndIndex);
				 //EndNode->GNode->ConnectNode(StartNode->GNode->shared_from_this(),StartIndex,EndIndex);
				
			 }
			 else if (StartSlotType == Output && 
				 StartNode->GNode->Output.at(StartIndex).VariableType == EndNode->GNode->Input.at(EndIndex).VariableType){


				 //TODO HERE : i have to allow for a list for output -> input
				 if (EndNode->vInputs.at(EndIndex).conn != nullptr) {
					 EndNode->vInputs.at(EndIndex).conn->ResetConnection();
				 }
				 if (StartNode->vOutputs.at(StartIndex).conn != nullptr) {
					 StartNode->vOutputs.at(StartIndex).conn->ResetConnection();
				 }

				 StartNode->vOutputs.at(StartIndex).SetCoords(InitDrawingPos, &EndNode->vInputs.at(EndIndex));
				 EndNode->vInputs.at(EndIndex).SetCoords(EndDrawingPos, &StartNode->vOutputs.at(StartIndex));
			//	 StartNode->GNode->ConnectNode(EndNode->GNode->shared_from_this(), EndIndex, StartIndex);
				 Graph::getInstance()->CreateConnectionOutIn(StartNode->GNode->shared_from_this(), EndNode->GNode, StartIndex, EndIndex);
			 }
			
			
		
		
		 }
		 //if the mouse is released when drawing a temporary line then remove the line from the initial point 
		 else if( ItemsHovered==false){


			 if (StartSlotType == Input) {
				 if (StartNode->vInputs.at(StartIndex).connected) {

					 auto x = StartNode->GNode->Input.at(StartIndex);
					 std::shared_ptr<Node> previousEndNode = x.ConnectedNode;
					 int previousEndIndex = x.ConnectionIndex;
					
					 Graph::getInstance()->RemoveConnection(previousEndNode,StartNode->GNode,previousEndIndex, StartIndex);
					 StartNode->vInputs.at(StartIndex).conn->ResetConnection();
					 StartNode->vInputs.at(StartIndex).ResetConnection();
					 
				 }
			 }
			 else if (StartSlotType == Output) {
				 if (StartNode->vOutputs.at(StartIndex).connected){

					 auto x = StartNode->GNode->Output.at(StartIndex);
					 std::shared_ptr<Node> previousEndNode = x.ConnectedNode;
					 int previousEndIndex = x.ConnectionIndex;

					 Graph::getInstance()->RemoveConnection(StartNode->GNode,previousEndNode, StartIndex, previousEndIndex);

					 StartNode->vOutputs.at(StartIndex).conn->ResetConnection();
					 StartNode->vOutputs.at(StartIndex).ResetConnection();
				 }
			 }
			
		 }
		 ResetGUITempInfo();

		 auto graph = Graph::getInstance();
		 graph->UpdateGraph();
		

	 }


}

void GUIManager::SetupGUI(GLFWwindow* window)
{
	// NodeView Panel position and size
	NodeViewPos = ImVec2(500, 200);
	NodeViewSize = ImVec2(800, 600);

	// Setup ImGui binding
	
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	ImGui_ImplGlfwGL3_Init(window, true);
}

void GUIManager::RenderGUI() {

	bool windowOpen = true;
	auto graph = Graph::getInstance();
	//new frame 
	ImGui_ImplGlfwGL3_NewFrame();

	//demo window
	ImGui::ShowDemoWindow();

	//Nodeview window setup
	ImGui::SetNextWindowPos(NodeViewPos, 0, ImVec2(0.0, 0.0)); // set next window position. call before Begin(). use pivot=(0.5f,0.5f) to center on given point, etc.
	ImGui::SetNextWindowSize(NodeViewSize, 0);    // set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin()
	ImGui::Begin("Node View", &windowOpen, 0);
	//the drawlist in the main GUI Manager for the node view
	ImDrawList* drawList = ImGui::GetWindowDrawList();

	

	//Drawing Visual Nodes
	
	for (std::vector<std::shared_ptr<VisualNode>>::iterator it = VNodeList.begin(); it != VNodeList.end(); ++it) {

		(*it)->DisplayNode(drawList, ImVec2(0, 0));
	}

	//Drawing of connections 
	RenderDrawing(drawList);

	//if values were updated,recompile
	if (ValueChanged) {
		
		graph->UpdateGraph();
		//std::cout << ValueChanged << std::endl;
	
	}

	bool OpenCreationMenu = false;
	// Open creation menu 
	if (!ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow() && ImGui::IsMouseClicked(1))
	{
		OpenCreationMenu = true;
	}
	if (OpenCreationMenu)
	{
		ImGui::OpenPopup("Creation Menu");
		
	}

	// Draw context menu
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("Creation Menu"))
	{

		//Mouse pos on popup open - the window's coordinates to have the relevant mouse position
		ImVec2 MousePos = ImGui::GetMousePosOnOpeningCurrentPopup() - NodeViewPos;
			ImGui::Separator();

			if (ImGui::BeginMenu("Input Node")) {
				for (std::map<std::string, InputNodeInformation>::iterator iter = graph->InputNodes.begin(); iter != graph->InputNodes.end(); ++iter)
				{

					//new menu entry
					if (ImGui::MenuItem((iter->first).c_str())) {
						//call constructor for InputNode
						CreateNode(MousePos, InputnodeT, iter->first);
					}

					//Key k = iter->first;
					//ignore value
					//Value v = iter->second;
				}

				if (ImGui::MenuItem("Time")) {
					//call constructor for function node
					CreateNode(MousePos, InputnodeT,"time");
				}
				ImGui::EndMenu();
			}
			
			if (ImGui::BeginMenu("Function Node")) {


				for (std::map<std::string,FunctionNodeInformation>::iterator iter = graph->FunctionNodes.begin(); iter != graph->FunctionNodes.end(); ++iter)
				{
					
					//new menu entry
					if (ImGui::MenuItem((iter->first).c_str())) {
						
						//call constructor for function node
						CreateNode(MousePos,FunctionnodeT,iter->first);
					}

					//Key k = iter->first;
					//ignore value
					//Value v = iter->second;
				}
				//-------- Iterate here 
			
				//ImGui::MenuItem("Multiplication Node");
				//-----------------------------

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Output Node")) {

				//-------- Iterate here 
				ImGui::MenuItem("Fragment Node");

				//-------------
				ImGui::EndMenu();
			}
	
			
		
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();


	//Reset stuff
	
	EndSlotType = DefaultNodeSlotType;
	ItemsHovered = false;
	ValueChanged = false;
	ImGui::End();
}

void GUIManager::RenderCall() {

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}

void GUIManager::DrawHermite(ImDrawList * drawList, ImVec2 p1, ImVec2 p2, int steps)
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

void GUIManager::ResetGUITempInfo()
{
	StartSlotType = EndSlotType = NodeSlotType::DefaultNodeSlotType;
	//InitDrawingPos = EndDrawingPos = ImVec2(-1000, -1000);
	StartNode = nullptr;
	EndNode = nullptr;
	
}

void GUIManager::ShowCreationMenu()
{


}

std::string GUIManager::GetSlotValueName(std::string Name,ValueType type)
{

	switch (type){
		case (Float):
			return (Name.append(" (Float)"));
			break;
		case (Int):
			return (Name.append(" (Int)"));
			break;
		case (Vec2):
			return (Name.append(" (Vec2)"));
			break;
		case (Vec3):
			return (Name.append(" (Vec3)"));
			break;
		case (Vec4):
			return (Name.append(" (Vec4)"));
			break;
		case (Mat4):
			return (Name.append(" (Mat4)"));
			break;
		case (Sampler2D):
			return (Name.append(" (Sample2D)"));
			break;
		case(SamplerCube):
			return (Name.append(" (SamplerCube)"));
			break;
		
		default :
			return (Name.append(" (Default)"));
			break;
	}
}

//void GUIManager::ResetVNodeDifs()
//{
//	//for (std::vector<ConnectionVCoords>::iterator it = PermanentLines.begin(); it != PermanentLines.end(); ++it) {
//	for (auto & it : PermanentLines){
//	//it->StartNode->PosDif;
//		ResetConnection(&it);
//	}
//}
//
//void GUIManager::UpdateConnection(ConnectionVCoords* connection)
//{
//	if (connection->SNP != nullptr && connection->ENP != nullptr) {
//
//
//		connection->Start = ImVec2(connection->Start.x + (connection->SNP->PosDif).x, connection->Start.y + (connection->SNP->PosDif).y);
//		connection->End = ImVec2(connection->End.x + (connection->ENP->PosDif).x, connection->End.y + (connection->ENP->PosDif).y);
//	}
//}
//
//void GUIManager::ResetConnection(ConnectionVCoords* connection)
//{
//	 connection->SNP->PosDif = ImVec2();
//	 connection->ENP->PosDif = ImVec2();
//
//}


