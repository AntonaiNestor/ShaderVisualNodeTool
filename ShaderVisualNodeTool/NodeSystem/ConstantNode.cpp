#include "ConstantNode.h"



ConstantNode::ConstantNode(float a, float x , float y)
{
	//general node attributes
	Name = "Constant";
	Type = NodeType::InputNode;
	UniqueID = Graph::getInstance()->AssignID();
	//varname might be important to check before assigning
	varName = "ConstantFloat";
	value = a;
	HasCompiled = false;
	NodePos = glm::vec2 (x,y);

	//output struct creation
	Connection connect;
	connect.ConnectedNode = nullptr;
	connect.ConnectionIndex = -1;
	connect.Value = a;
	connect.Enabled = true;

	Output.push_back(connect);
	
    Graph::getInstance()->AddConstant(this);

}

ConstantNode::~ConstantNode()
{

}

//
// Compiling constant nodes means only appending a simple var in the shadercode
// The actual value of the node is also passed to the children nodes on connection
void ConstantNode::Compile(std::string	*ShaderCode) {
	ShaderCode->append("\n" + CodeString());
	HasCompiled = true;
	//DrawNode(true);
}

std::string ConstantNode::CodeString()
{
	return "$" + varName + " = " + std::to_string(value) + ";";
}

void ConstantNode::DrawNode(bool active)
{
	static bool inited = false;
	static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
	static bool show_grid = true;
	static int node_selected = -1;
	
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImGui::PushID(this->UniqueID);
	ImVec2 node_rect_min = ImVec2(NodePos.x,NodePos.y); //node pos = mouse pos on creation -> pass it initially from constructor
	ImVec2 node_rect_max = ImVec2(node_rect_min.x + 100, node_rect_min.y + 100);
	// Display node contents first
	draw_list->ChannelsSetCurrent(1); // Foreground
	bool old_any_active = ImGui::IsAnyItemActive();
	//ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);

	ImGui::BeginGroup(); // Lock horizontal position
	ImGui::Text("%s",Name); // display name 
	ImGui::SliderFloat("##value", &value, 0.0f, 1.0f, "Value: %.2f"); //display value of the node
	//ImGui::ColorEdit3("##color", &node->Color.x);
	ImGui::EndGroup();

	// Save the size of what we have emitted and whether any of the widgets are being used
	bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());
	//node->Size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
	//ImVec2 node_rect_max = node_rect_min + node->Size;
		
	// Display node box
	draw_list->ChannelsSetCurrent(0); // Background
	ImGui::SetCursorScreenPos(node_rect_min);
	//ImGui::InvisibleButton("node", node->Size);

	bool node_moving_active = ImGui::IsItemActive();
	if (node_widgets_active || node_moving_active)
		node_selected = UniqueID;
	if (node_moving_active && ImGui::IsMouseDragging(0))
		//node->Pos = node->Pos + ImGui::GetIO().MouseDelta;*/

	//ImU32 node_bg_color = (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_selected == node->ID)) ? IM_COL32(75, 75, 75, 255) : IM_COL32(60, 60, 60, 255);
	draw_list->AddRectFilled(node_rect_min, node_rect_min, IM_COL32(75, 75, 75, 255), 4.0f);
	draw_list->AddRect(node_rect_min, node_rect_max, IM_COL32(100, 100, 100, 255), 4.0f);
	/*for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++)
		draw_list->AddCircleFilled(offset + node->GetInputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
	for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++)
		draw_list->AddCircleFilled(offset + node->GetOutputSlotPos(slot_idx), NODE_SLOT_RADIUS, IM_COL32(150, 150, 150, 150));
*/
	ImGui::PopID();

	draw_list->ChannelsMerge();

}



