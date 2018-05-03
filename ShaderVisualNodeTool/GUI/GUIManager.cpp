#include "GUIManager.h"
#include "../NodeSystem/ConstantNode.h"






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
	StartSlotType = EndSlotType = Default;
}
//
//
//GUIManager::~GUIManager()
//{
//}


void GUIManager::CreateNode(ImVec2 pos)
{
	// 1) Create Graph Node   
	// Creation of the object in the heap, using make shared. The pointer is local in the stack but 
	// since we push the object in the graph's list then a reference is kept.
	
	std::shared_ptr<Node> newGraphNode = std::make_shared<ConstantNode>();
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
		 //if valid new connection
		 if ((StartSlotType == Input && EndSlotType == Output) ||
			 (StartSlotType == Output && EndSlotType == Input)) {
			
			 

			//add visual coordinates in permanentLines
			 PermanentLines.push_back(ConnectionVCoords(InitDrawingPos, EndDrawingPos));
			 ResetGUITempInfo();
		 
			 //create graph connection between Nodes


		 }
	 }

	 //draw all permanent lines - if there are 
	 if (!PermanentLines.empty()) {

		 
		 for (std::vector<ConnectionVCoords>::iterator it = PermanentLines.begin(); it != PermanentLines.end(); ++it) {
			 DrawHermite(drawlist, it->Start,it->End, 20);
		 }
		
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

	//new frame 
	ImGui_ImplGlfwGL3_NewFrame();

	//demo window
	ImGui::ShowDemoWindow();

	//Nodeview window setup
	ImGui::SetNextWindowPos(NodeViewPos, 0, ImVec2(0.0, 0.0)); // set next window position. call before Begin(). use pivot=(0.5f,0.5f) to center on given point, etc.
	ImGui::SetNextWindowSize(NodeViewSize, 0);    // set next window size. set axis to 0.0f to force an auto-fit on this axis. call before Begin()
	ImGui::Begin("Node View", &windowOpen, 2);
	//the drawlist in the main GUI Manager for the node view
	ImDrawList* drawList = ImGui::GetWindowDrawList();


	//TRAVERSE GRAPH HERE for nodes
//	vnode->DisplayNode(drawList, ImVec2(0, 0));
	for (std::vector<std::shared_ptr<VisualNode>>::iterator it = VNodeList.begin(); it != VNodeList.end(); ++it) {
		
		(*it)->DisplayNode(drawList, ImVec2(0, 0));
	}


	RenderDrawing(drawList);

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
	StartSlotType = EndSlotType = Default;
	InitDrawingPos = EndDrawingPos = ImVec2(-1000, -1000);
	
}
