#pragma once


#include "Imgui/imgui.h"
#include "../NodeSystem/Node.h"
#include "Imgui/imgui_impl_glfw_gl3.h"
#include "Imgui/imgui_internal.h"
#include "Imgui/imconfig.h"
#include "VisualNode.h"

class VisualNode;

class GUIManager
{
public:

	//attributes
	ImVec2 NodeViewPos;
	ImVec2 NodeViewSize;

	ImGuiContext* Context; 
	ImGuiIO io;
	//methods
	VisualNode* vnode;

	GUIManager();
	~GUIManager();


	void SetupGUI(GLFWwindow* window);
	void RenderGUI();
	void RenderCall();
};

