#pragma once

#include "../Node.h"
#include <algorithm>
#include "../Log.h"
#include <mutex>

class NodeAddFloat : public Node
{
protected:
	
	// The offset between the left and right size so they dont touch
	float middleOffset = 10.f;
	// The default size of the left to middle and the right to middle
	float inputDefaultSize = 50.f;

	// The radius of the connector pins
	float pinRadius = 4.f;

	// the padding size of the node window
	ImVec2 windowPadding{ 8.f, 8.f };
public:
	NodeAddFloat() = delete;
	NodeAddFloat(ImVec2 nodePos)
	{
		name = "Add Float";
		pos = nodePos;
		size = { 270, 80 };
		inputSlots.emplace_back(ADD_SLOT_FLOAT("A", id));
		inputSlots.emplace_back(ADD_SLOT_FLOAT("B", id));
		outputSlots.emplace_back(ADD_SLOT_FLOAT("Out", id));
		//middleOffset = 10.f;
		//inputDefaultSize = 50.f;
	}
	virtual ~NodeAddFloat() = default;

	virtual std::optional<std::any> EvaluateNode() override
	{
		return std::make_any<int>(0);
	}

	virtual void DrawNodeProperties() override
	{
		ImGui::Text("%s", name.c_str());
		ImGui::InputFloat2("Pos", &pos[0]);
		ImGui::InputFloat2("Size", &size[0]);

		ImGui::DragFloat("Input Slot Length", &maxInputSlotLength);
		ImGui::DragFloat("Output Slot Length", &maxOutputSlotLength);

		ImGui::DragFloat("Middle Offset", &middleOffset);
		ImGui::DragFloat("Input Default Size", &inputDefaultSize);

		ImGui::DragFloat("Pin Radius", &pinRadius);
		ImGui::DragFloat2("Window Padding", &windowPadding.x);
	}

	virtual void DrawNode(ImDrawList * drawList, const ImVec2& offset,
						  NodeID * nodeSelected) override
	{
		int node_hovered_in_list = -1;
		int node_hovered_in_scene = -1;

		ImGui::PushID((int)id);
		ImVec2 nodeRectMin = offset + pos;

		drawList->ChannelsSetCurrent(1); // Foreground
		bool old_any_active = ImGui::IsAnyItemActive();

		float textHeight = ImGui::GetTextLineHeight();
		float textHeightSpacing = ImGui::GetTextLineHeightWithSpacing();
		float textSpacing = textHeightSpacing - textHeight;

		ImVec2 nodeItemStartPos = nodeRectMin + windowPadding;
		ImVec2 pinOffset = ImVec2(0.f, textHeight + (2 * textSpacing));
		ImVec2 pinStartPos = nodeItemStartPos + pinOffset;

		ImGui::SetCursorScreenPos(pinStartPos);

		ImGui::BeginGroup();
		// Skip the title, we draw that after
		auto inputSlotsSize = (int) inputSlots.size();
		auto outputSlotsSize = (int) outputSlots.size();
		static auto maxSlots = std::max(inputSlotsSize, outputSlotsSize);

		CalcMaxSlotLengths();

		// How long is the entire thing
		auto leftSideLen = std::max(inputDefaultSize, maxInputSlotLength + middleOffset);
		auto rightSideLen = std::max(inputDefaultSize, maxOutputSlotLength + middleOffset);
		float maxItemWidth = leftSideLen + rightSideLen;

		static std::vector<ImVec2> slotPositions;
		slotPositions.clear();
		slotPositions.reserve(inputSlotsSize + outputSlotsSize);

		for (int i = 0; i < maxSlots; ++i)
		{
			// Draw it if it exists
			if (i < inputSlots.size())
			{
				slotPositions.emplace_back(ImGui::GetCursorScreenPos() +
									ImVec2(-windowPadding.x, textHeight / 2.f));
				inputSlots[i].DrawInputSlot();
			}

			if (i < outputSlots.size())
			{
				// move the cursor to the right side
				ImGui::SameLine(maxItemWidth - outputSlots[i].slotLength);
				slotPositions.emplace_back(ImGui::GetCursorScreenPos() +
					ImVec2(outputSlots[i].slotLength + windowPadding.x, textHeight / 2.f));
				outputSlots[i].DrawOutputSlot();
			}
		}

		auto textSize = ImGui::CalcTextSize(name.c_str());
		auto spacing = (maxItemWidth - textSize.x) / 2.f;

		ImGui::SetCursorScreenPos(nodeItemStartPos + ImVec2(spacing, 0));
		ImGui::Text("%s", name.c_str());
		ImGui::EndGroup();

		size = ImGui::GetItemRectSize() + windowPadding + windowPadding + pinOffset;
		ImVec2 nodeRectMax = nodeRectMin + size;

		bool node_widgets_active = (!old_any_active && ImGui::IsAnyItemActive());

		// Display node box
		drawList->ChannelsSetCurrent(0); // Background
		ImGui::SetCursorScreenPos(nodeRectMin);
		ImGui::InvisibleButton("node", size);
		if (ImGui::IsItemHovered())
		{
			node_hovered_in_scene = (int)id;
			//open_context_menu |= ImGui::IsMouseClicked(1);
		}
		bool node_moving_active = ImGui::IsItemActive();
		if (node_widgets_active || node_moving_active)
			*nodeSelected = id;
		if (node_moving_active && ImGui::IsMouseDragging(0))
		{
			pos.x += ImGui::GetIO().MouseDelta.x;
			pos.y += ImGui::GetIO().MouseDelta.y;
		}

		drawList->AddRectFilled(nodeRectMin, nodeRectMax,
			(node_hovered_in_list == id || node_hovered_in_scene == id || 
			(node_hovered_in_list == -1 && *nodeSelected == id)) ? 
				hoverColor : backgroundColor, cornerRounding);

		drawList->AddRect(nodeRectMin, nodeRectMax, outlineColor, cornerRounding);

		ImVec2 lineDividerStart = pinStartPos - ImVec2(windowPadding.x, textSpacing);
		ImVec2 lineDividerEnd = lineDividerStart + ImVec2(size.x, 0);
		
		drawList->AddLine(lineDividerStart, lineDividerEnd, outlineColor);

		for (auto slotPosition : slotPositions)
		{
			drawList->AddCircleFilled(slotPosition, pinRadius,
									  ImColor(150, 150, 150, 150));
		}

		ImGui::PopID();
	}
};