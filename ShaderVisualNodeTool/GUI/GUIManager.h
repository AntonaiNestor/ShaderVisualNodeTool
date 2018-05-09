#pragma once


#include "Imgui/imgui.h"
#include "../NodeSystem/Node.h"
#include "Imgui/imgui_impl_glfw_gl3.h"
#include "Imgui/imgui_internal.h"
#include "Imgui/imconfig.h"
#include "VisualNode.h"

class VisualNode; 

enum NodeSlotType{
	Default,
	Input,
	Output
};



class GUIManager
{
public:

	//----------- singleton stuff-------
	static GUIManager* getInstance();
	GUIManager();
	GUIManager(GUIManager const&);              // Don't Implement
	void operator=(GUIManager const&); // Don't implement


	//attributes
	ImVec2 NodeViewPos;
	ImVec2 NodeViewSize;

	ImGuiContext* Context; 
	ImGuiIO io;

	bool IsDrawing=false;
	bool DrawPermanent = false;
	//Drawing Connections

	//Coordinates
	ImVec2 InitDrawingPos;
	ImVec2 EndDrawingPos;
	//Types for matching inputs to outputs
	NodeSlotType StartSlotType,EndSlotType;
	VisualNode* StartNode;
	VisualNode* EndNode;
	int StartIndex, EndIndex;
	//Structure of the final thing
	//std::vector<ConnectionVCoords> PermanentLines;
	

	//Keep a list with all the visual nodes. Essentially we keep double the information. 
	std::vector<std::shared_ptr<VisualNode>> VNodeList;

	//methods
	

	/*GUIManager();
	~GUIManager();*/

	//Node creation methods

	//create node - NEEDS TO PAY ATTENTION TO THE TYPE AND CREATE ACCORDINGLY
	void CreateNode(ImVec2 pos);

	//General GUI manager 
	void RenderDrawing(ImDrawList* drawlist);
	void SetupGUI(GLFWwindow* window);
	void RenderGUI();
	void RenderCall();
	void DrawHermite(ImDrawList* drawList, ImVec2 p1, ImVec2 p2, int steps);
	void ResetGUITempInfo();
	void ResetVNodeDifs();

	//void UpdateConnection(ConnectionVCoords* connection);
	//void ResetConnection(ConnectionVCoords* connection);

private:
	static GUIManager* Instance;

};

