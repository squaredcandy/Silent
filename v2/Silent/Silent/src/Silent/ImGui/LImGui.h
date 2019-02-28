#pragma once
#include "../Core.h"
#include "../Layer/Layer.h"
#include <ImGui/imgui.h>

namespace Silent
{
	class SILENT_API LPlatform;
	class SILENT_API LRenderer;
	template<class Platform, class Renderer>
	class SILENT_API LImGui : public Layer
	{
	protected:
		Platform * _platform;
		Renderer * _renderer;
	public:
		LImGui(Platform * platform = nullptr, Renderer * renderer = nullptr);
		~LImGui() = default;

		virtual void OnInitialize() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;

		void NewFrame();
		void Render();
		void ChangeColors();
		void AddBuffer(ImTextureID textureID, glm::vec2 size, glm::vec2 pos);
	};

	template<class Platform, class Renderer>
	void LImGui<Platform, Renderer>::AddBuffer(ImTextureID textureID, 
											   glm::vec2 size, glm::vec2 pos)
	{
		static auto style = ImGui::GetStyle();
		ImVec2 start = { pos.x, pos.y };
		ImVec2 end = { pos.x + size.x, pos.y + size.y };
		ImGui::GetWindowDrawList()->AddImageRounded(textureID, 
			start, end, { 0, 1 }, { 1, 0 }, 0xFFFFFFFF, style.WindowRounding);
	}

	template<class Platform, class Renderer>
	void LImGui<Platform, Renderer>::ChangeColors()
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

	template<class Platform, class Renderer>
	LImGui<Platform, Renderer>::LImGui(Platform * platform, Renderer * renderer)
		: _platform(platform), _renderer(renderer)
	{

	}
}

