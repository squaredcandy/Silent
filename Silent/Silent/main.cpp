#include "nlohmann/json.hpp"
using namespace nlohmann;

#include <iostream>

#include "Engine.h"
#include "TestSystem.h"
#include "Log.h"
#include "Multithread.h"
#include "NodeGraph.h"
#include "Widgets.h"

using namespace Silent::Engine;

//#include <ImGui/imgui_nodegrapheditor.h>
// NB: You can use math functions/operators on ImVec2 if you #define IMGUI_DEFINE_MATH_OPERATORS and       #include "imgui_internal.h"
// Here we only declare simple +/- operators so others don't leak into the demo code.
#ifndef IMGUI_DEFINE_MATH_OPERATORS
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + /rhs.y); }
static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
#endif // !IMGUI_DEFINE_MATH_OPERATORS
// Really dumb data structure provided for the example.
// Note that we storing links are INDICES (not ID) to make example code shorter, obviously a bad idea for any general purpose code.
static void ShowExampleAppCustomNodeGraph(bool* opened)
{
	ImGui::SetNextWindowSize(ImVec2(700, 600), ImGuiCond_FirstUseEver);
	if (!ImGui::Begin("Example: Custom Node Graph", opened))
	{
		ImGui::End();
		return;
	}

	struct Node
	{
		int     ID;
		char    Name[32];
		ImVec2  Pos, Size;
		float   Value;
		int     InputsCount, OutputsCount;

		Node(int id, const char* name, const ImVec2& pos, float value, int inputs_count, int outputs_count) { ID = id; strncpy(Name, name, 31); Name[31] = 0; Pos = pos; Value = value; InputsCount = inputs_count; OutputsCount = outputs_count; }

		ImVec2 GetInputSlotPos(int slot_no) const { return ImVec2(Pos.x, Pos.y + Size.y * ((float) slot_no + 1) / ((float) InputsCount + 1)); }
		ImVec2 GetOutputSlotPos(int slot_no) const { return ImVec2(Pos.x + Size.x, Pos.y + Size.y * ((float) slot_no + 1) / ((float) OutputsCount + 1)); }
	};
	struct NodeLink
	{
		int     InputIdx, InputSlot, OutputIdx, OutputSlot;

		NodeLink(int input_idx, int input_slot, int output_idx, int output_slot) { InputIdx = input_idx; InputSlot = input_slot; OutputIdx = output_idx; OutputSlot = output_slot; }
	};

	static ImVector<Node> nodes;
	static ImVector<NodeLink> links;
	static bool inited = false;
	static ImVec2 scrolling = ImVec2(0.0f, 0.0f);
	static int node_selected = -1;
	if (!inited)
	{
		nodes.push_back(Node(0, "MainTex", ImVec2(40, 50), 0.5f, 1, 1));
		nodes.push_back(Node(1, "BumpMap", ImVec2(40, 150), 0.42f, 1, 1));
		nodes.push_back(Node(2, "Combine", ImVec2(270, 80), 1.0f, 2, 2));
		links.push_back(NodeLink(0, 0, 2, 0));
		links.push_back(NodeLink(1, 0, 2, 1));
		inited = true;
	}

	// Draw a list
	bool open_context_menu = false;
	int node_hovered_in_list = -1;
	int node_hovered_in_scene = -1;
	ImGui::BeginChild("node_list", ImVec2(100, 0));
	ImGui::Text("Nodes");
	ImGui::Separator();
	for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
	{
		Node* node = &nodes[node_idx];
		ImGui::PushID(node->ID);
		if (ImGui::Selectable(node->Name, node->ID == node_selected))
			node_selected = node->ID;
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_list = node->ID;
			open_context_menu |= ImGui::IsMouseClicked(1);
		}
		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::SameLine();
	ImGui::BeginGroup();

	const float NODE_SLOT_RADIUS = 4.0f;
	const ImVec2 NODE_WINDOW_PADDING(8.0f, 8.0f);

	ImGui::Text("Hold middle mouse button to scroll (%.2f,%.2f)", scrolling.x, scrolling.y);
	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1, 1));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::PushStyleColor(ImGuiCol_ChildBg, (ImVec4)ImColor(40, 40, 40, 200));
	ImGui::BeginChild("scrolling_region", ImVec2(0, 0), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove);
	ImGui::PushItemWidth(120.0f);

	ImDrawList* draw_list = ImGui::GetWindowDrawList();
	draw_list->ChannelsSplit(2);
	ImVec2 offset = ImGui::GetCursorScreenPos() - scrolling;

	// Display links
	draw_list->ChannelsSetCurrent(0); // Background
	for (int link_idx = 0; link_idx < links.Size; link_idx++)
	{
		NodeLink* link = &links[link_idx];
		Node* node_inp = &nodes[link->InputIdx];
		Node* node_out = &nodes[link->OutputIdx];

#if 1
		// Hermite spline
		// TODO: move to Path API
		ImVec2 p1 = offset + node_inp->GetOutputSlotPos(link->InputSlot);
		ImVec2 t1 = ImVec2(+80.0f, 0.0f);
		ImVec2 p2 = offset + node_out->GetInputSlotPos(link->OutputSlot);
		ImVec2 t2 = ImVec2(+80.0f, 0.0f);
		const int STEPS = 12;
		for (int step = 0; step <= STEPS; step++)
		{
			float t = (float) step / (float) STEPS;
			float h1 = +2 * t*t*t - 3 * t*t + 1.0f;
			float h2 = -2 * t*t*t + 3 * t*t;
			float h3 = t * t*t - 2 * t*t + t;
			float h4 = t * t*t - t * t;
			draw_list->PathLineTo(ImVec2(h1*p1.x + h2 * p2.x + h3 * t1.x + h4 * t2.x, h1*p1.y + h2 * p2.y + h3 * t1.y + h4 * t2.y));
		}
		draw_list->PathStroke(ImColor(200, 200, 100), false, 3.0f);
#else
		draw_list->AddLine(offset + node_inp->GetOutputSlotPos(link->InputSlot), offset + node_out->GetInputSlotPos(link->OutputSlot), ImColor(200, 200, 100), 3.0f);
#endif
	}

	// Display nodes
	for (int node_idx = 0; node_idx < nodes.Size; node_idx++)
	{
		Node* node = &nodes[node_idx];
		ImGui::PushID(node->ID);

		ImVec2 node_rect_min = offset + node->Pos;

		// Display node contents first
		draw_list->ChannelsSetCurrent(1); // Foreground

		bool old_any_active = ImGui::IsAnyItemActive();
		ImGui::SetCursorScreenPos(node_rect_min + NODE_WINDOW_PADDING);
		ImGui::BeginGroup(); // Lock horizontal position
		ImGui::Text("%s", node->Name);
		ImGui::SliderFloat("##value", &node->Value, 0.0f, 1.0f, "Alpha %.2f");
		float dummy_color[3] = { node->Pos.x / ImGui::GetWindowWidth(), node->Pos.y / ImGui::GetWindowHeight(), fmodf((float) node->ID * 0.5f, 1.0f) };
		ImGui::ColorEdit3("##color", &dummy_color[0]);
		ImGui::EndGroup();

		node->Size = ImGui::GetItemRectSize() + NODE_WINDOW_PADDING + NODE_WINDOW_PADDING;
		ImVec2 node_rect_max = node_rect_min + node->Size;

		bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());

		// Display node box
		draw_list->ChannelsSetCurrent(0); // Background
		ImGui::SetCursorScreenPos(node_rect_min);
		ImGui::InvisibleButton("node", node->Size);
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_scene = node->ID;
			open_context_menu |= ImGui::IsMouseClicked(1);
		}
		bool node_moving_active = ImGui::IsItemActive();
		if (node_widgets_active || node_moving_active)
			node_selected = node->ID;
		if (node_moving_active && ImGui::IsMouseDragging(0))
		{
			node->Pos.x += ImGui::GetIO().MouseDelta.x;
			node->Pos.y += ImGui::GetIO().MouseDelta.y;
		}

		draw_list->AddRectFilled(node_rect_min, node_rect_max, (node_hovered_in_list == node->ID || node_hovered_in_scene == node->ID || (node_hovered_in_list == -1 && node_selected == node->ID)) ? ImColor(75, 75, 75) : ImColor(60, 60, 60), 4.0f);
		draw_list->AddRect(node_rect_min, node_rect_max, ImColor(100, 100, 100), 4.0f);
		for (int slot_idx = 0; slot_idx < node->InputsCount; slot_idx++)
		{
			ImVec2 pos = node->GetInputSlotPos(slot_idx);
			draw_list->AddCircleFilled(offset + pos, NODE_SLOT_RADIUS, ImColor(150, 150, 150, 150));
		}
		for (int slot_idx = 0; slot_idx < node->OutputsCount; slot_idx++)
		{
			ImVec2 pos = node->GetOutputSlotPos(slot_idx);
			draw_list->AddCircleFilled(offset + pos, NODE_SLOT_RADIUS, ImColor(150, 150, 150, 150));
		}

		ImGui::PopID();
	}
	draw_list->ChannelsMerge();

	// Open context menu
	if (!ImGui::IsAnyItemHovered() && /*ImGui::IsMouseHoveringWindow()*/
		ImGui::IsWindowHovered() && ImGui::IsMouseClicked(1))
	{
		node_selected = node_hovered_in_list = node_hovered_in_scene = -1;
		open_context_menu = true;
	}
	if (open_context_menu)
	{
		ImGui::OpenPopup("context_menu");
		if (node_hovered_in_list != -1)
			node_selected = node_hovered_in_list;
		if (node_hovered_in_scene != -1)
			node_selected = node_hovered_in_scene;
	}

	// Draw context menu
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
	if (ImGui::BeginPopup("context_menu"))
	{
		Node* node = node_selected != -1 ? &nodes[node_selected] : NULL;
		ImVec2 scene_pos = ImGui::GetMousePosOnOpeningCurrentPopup() - offset;
		if (node)
		{
			ImGui::Text("Node '%s'", node->Name);
			ImGui::Separator();
			if (ImGui::MenuItem("Rename..", NULL, false, false)) {}
			if (ImGui::MenuItem("Delete", NULL, false, false)) {}
			if (ImGui::MenuItem("Copy", NULL, false, false)) {}
		}
		else
		{
			if (ImGui::MenuItem("Add")) { nodes.push_back(Node(nodes.Size, "New node", scene_pos, 0.5f, 2, 2)); }
			if (ImGui::MenuItem("Paste", NULL, false, false)) {}
		}
		ImGui::EndPopup();
	}
	ImGui::PopStyleVar();

	// Scrolling
	if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(2, 0.0f))
	{
		scrolling.x -= ImGui::GetIO().MouseDelta.x;
		scrolling.y -= ImGui::GetIO().MouseDelta.y;
	}

	ImGui::PopItemWidth();
	ImGui::EndChild();
	ImGui::PopStyleColor();
	ImGui::PopStyleVar(2);
	ImGui::EndGroup();

	ImGui::End();
}

void LoadResources()
{
	Multithread::CreateThread("Load Resources", Resources::LoadAllResources);
	Systems::AddSystem<TestSystem>();
	auto obj1 = Entities::AddEntity("Object 1");
	Entities::AddEntity("Object 2", obj1->GetChildren());
	NodeGraph::ExampleNodes();
}

void Loop()
{
	//Multithread::ManageThreads();

	if (ImGui::IsKeyPressed(KEYCODE_N))
	{
		LOG_INFO("Added nodes");
		/*for(int i = 0; i < 20; ++i) */NodeGraph::ExampleNodes();
	}
}

static bool a = true;

void Window()
{
	Widget::MainDockspace(&a);
	ImGui::ShowDemoWindow(&a);

	Widget::DrawLog();
	Widget::Stats(&a);
	Widget::ObjectInspector(&a);
	Widget::ConfigureSettings(&a);

	Widget::DrawNodeGraphProperties(&a);
	Widget::DrawNodeGraph(&a);
}

void ChangeColors()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.13f, 0.13f, 0.13f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.76f, 0.76f, 0.76f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.99f, 0.99f, 0.99f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.87f, 0.87f, 0.87f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.74f, 0.74f, 0.74f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.68f, 0.68f, 0.68f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(1.00f, 0.61f, 0.00f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(1.00f, 0.69f, 0.20f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.00f, 0.76f, 0.40f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.34f, 0.34f, 0.34f, 0.25f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.46f, 0.46f, 0.46f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.23f, 0.23f, 0.23f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.27f, 0.27f, 0.27f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(1.00f, 0.65f, 0.10f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(1.00f, 0.61f, 0.00f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.08f, 0.08f, 0.08f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.89f, 0.55f, 0.00f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(1.00f, 0.61f, 0.00f, 1.00f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

int main(int argc, char ** argv)
{
	InitEngine();

	ChangeColors();

	Log::InitLog();
	
	LoadResources();

	loopFunction = &Loop;
	windowFunction = &Window;

	GameLoop();
	Cleanup();
	return 0;
}