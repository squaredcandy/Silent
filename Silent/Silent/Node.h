#pragma once

#include <vector>
#include <string>
#include <memory>
#include <any>
#include <optional>
#include <mutex>

#define IMGUI_DEFINE_MATH_OPERATORS
#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>
#include <algorithm>

enum NodeSlotType
{
	//SLOT_EXEC,
	SLOT_INT,
	SLOT_FLOAT,
	SLOT_DOUBLE,
	SLOT_STRING,
	SLOT_BOOL,
	SLOT_VEC2,
	SLOT_VEC3,
	SLOT_VEC4
};

#define SLOT_INVALID_LINK UINT_MAX

using NodeID = std::size_t;

class NodeSlot
{
public:
	std::string slotName;
	NodeSlotType slotType;

	// Make sure to update this info
	NodeID selfID;		// The ID of the node that owns this slot
	NodeID otherID; // The ID of the node this slot is connecting to

	float slotLength;

	NodeSlot(std::string name, NodeSlotType type, NodeID self, NodeID other) :
		slotName(name), slotType(type), selfID(self), otherID(other) 
	{
		//CalcInputSlotLength();
		//CalcOutputSlotLength();
	}

	virtual float CalcInputSlotLength()
	{
		slotLength = ImGui::CalcTextSize(slotName.c_str()).x;
		return slotLength;
	}

	virtual float CalcOutputSlotLength()
	{
		slotLength = ImGui::CalcTextSize(slotName.c_str()).x;
		return slotLength;
	}

	virtual float DrawInputSlot()
	{
		// Calculate the slot length once
		static std::once_flag flag;
		std::call_once(flag, [&] () { CalcInputSlotLength(); });

		ImGui::Text("%s", slotName.c_str());
		return slotLength;
	}

	virtual float DrawOutputSlot()
	{
		// Calculate the slot length once
		static std::once_flag flag;
		std::call_once(flag, [&] () { CalcOutputSlotLength(); });

		ImGui::Text("%s", slotName.c_str());
		return slotLength;
	}
};

#define ADD_SLOT_FLOAT(name, id) NodeSlot(name, SLOT_FLOAT, id, SLOT_INVALID_LINK)

inline NodeID GenerateNewNodeID()
{
	static NodeID lastId = 0;
	return lastId++;
}

class Node
{
protected:
	NodeID id = GenerateNewNodeID();
	std::string name;
	ImVec2 pos, size;
	std::vector<NodeSlot> inputSlots;
	std::vector<NodeSlot> outputSlots;

	float maxInputSlotLength;
	float maxOutputSlotLength;

	ImColor outlineColor = ImColor(100, 100, 100);
	ImColor backgroundColor = ImColor(60, 60, 60);
	ImColor hoverColor = ImColor(75, 75, 75);
	float cornerRounding = 4.f;
	
public:
	Node() = default;
	Node(Node&) = default;
	Node(Node&&) = default;
	//Node(std::string nodeName, ImVec2& nodePos) : name(nodeName), pos(nodePos) {}
	virtual ~Node() = default;

	virtual std::optional<std::any> EvaluateNode() = 0;
	virtual void DrawNodeProperties() = 0;
	virtual void DrawNode(ImDrawList * drawList, const ImVec2& offset,
						  NodeID * nodeSelected) = 0;

	virtual void CalcMaxSlotLengths()
	{
		for (auto& input : inputSlots)
		{
			maxInputSlotLength = 
				std::max(input.CalcInputSlotLength(), maxInputSlotLength);
		}

		for (auto& output : outputSlots)
		{
			maxOutputSlotLength = 
				std::max(output.CalcOutputSlotLength(), maxOutputSlotLength);
		}
	}

	ImVec2 GetInputSlotPos(int slot_no) const
	{
		return ImVec2(pos.x, pos.y + size.y * ((float) slot_no + 1) / 
			((float) inputSlots.size() + 1));
	}

	ImVec2 GetOutputSlotPos(int slot_no) const 
	{ 
		return ImVec2(pos.x + size.x, pos.y + size.y * ((float) slot_no + 1) / 
			((float) outputSlots.size() + 1));
	}
};