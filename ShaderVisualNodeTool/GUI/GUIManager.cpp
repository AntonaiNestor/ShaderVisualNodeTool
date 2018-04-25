#include "GUIManager.h"



GUIManager::GUIManager()
{
	Context = ImGui::CreateContext();
	io = ImGui::GetIO(); (void)io;
	
}


GUIManager::~GUIManager()
{
}

void GUIManager::SetupGUI(GLFWwindow * window)
{
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

	vnode->DisplayNode(drawList, ImVec2(0,0));


	ImGui::End();
}

void GUIManager::RenderCall() {

	ImGui::Render();
	ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());
}
