#include "Widgets.h"

#include <ImGui/imgui.h>

#include <vector>
#include <numeric>
#include <SDL/SDL.h>

#include "Log.h"
#include "Settings.h"

#define DT_LEN 50
#define IMGUI_NEWWINDOW ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver)

namespace Silent::Engine::Widget
{
	EntityID selectedID = UINT_MAX;

	void MainDockspace(bool* open)
	{
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", open, window_flags);
		ImGui::PopStyleVar();

		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Docking"))
			{

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
		ImGui::PopStyleVar(2);
		ImGui::End();
	}

	void ObjectHierarchyRecursive(std::vector<Entity *> entities)
	{
		for (const auto& e : entities)
		{
			auto id = e->GetID();
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_OpenOnDoubleClick |
				((selectedID == id) ? ImGuiTreeNodeFlags_Selected : 0);

			bool openNode = ImGui::TreeNodeEx((void*) id, flags,
											  e->GetCommon().name.c_str());
			if (ImGui::IsItemClicked())
			{
				if (selectedID == id) { selectedID = UINT_MAX; }
				else { selectedID = id; }
			}

			if (openNode)
			{
				ObjectHierarchyRecursive(Entities::GetAll(e->GetChildren()));
				ImGui::TreePop();
			}
		}
	}

	void ObjectProperties()
	{
		static Entity * currentEntity;
		currentEntity = Entities::Get(selectedID);
		if (currentEntity != nullptr)
		{
			ImGui::Text("Name: %s", currentEntity->GetCommon().name.c_str());
			currentEntity->GUI();
		}
		else
		{
			ImGui::Text("Nothing Selected");
		}
	}

	void ObjectInspector(bool * open)
	{
		IMGUI_NEWWINDOW;
		ImGui::Begin("Object Inspector", open);

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);
		ImGui::Separator();

		ObjectHierarchyRecursive(Entities::GetAll());

		ImGui::NextColumn();

		ObjectProperties();

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();

		ImGui::End();
	}

	void Stats(bool * open)
	{
		static Uint32 lastTime = SDL_GetTicks();
		static Uint32 totalTime = SDL_GetTicks();
		static Uint32 deltaTimes[DT_LEN];
		static int idx = 0;

		lastTime = totalTime;
		totalTime = SDL_GetTicks();
		deltaTimes[idx] = totalTime - lastTime;
		idx = (idx + 1) % DT_LEN;

		IMGUI_NEWWINDOW;
		ImGui::Begin("Stats", open);
		ImGui::Text("Total Time: %.1fs", totalTime / 1000.f);
		auto total = std::accumulate(deltaTimes, deltaTimes + DT_LEN, 0);
		ImGui::Text("Delta Time: %.3fms", ((float) total / (float) DT_LEN));
		ImGui::End();
	}

	void ConfigureSettings(bool * open)
	{
		using namespace Silent::Settings;
		ImGui::Begin("Settings", open);
		if (ImGui::Button("Save Settings"))
		{
			SaveSettings();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load Settings"))
		{
			LoadSettings();
		}
		if (ImGui::InputInt("Window Width", &windowWidth, 160, 160))
		{
			ChangeWindowWidth();
		}
		if (ImGui::InputInt("Window Height", &windowHeight, 90, 90))
		{
			ChangeWindowHeight();
		}
		if (ImGui::Checkbox("vSync", &vSync))
		{
			ChangeVSync();
		}
		ImGui::End();
	}

	void DrawLog()
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		ImGui::Begin("Log", &Settings::showLog);
		ImGui::BeginChild("Scrolling", ImVec2(0, 0), false,
						  ImGuiWindowFlags_HorizontalScrollbar);
		for (const auto& entry : Log::logHistory)
		{
			std::string info;
			// Up to here - Adding color to log
			switch (entry.infoLevel)
			{
			case TRACE_LEVEL:
				info = "[TRACE]";
				break;
			case INFO_LEVEL:
				info = "[INFO]";
				break;
			case WARN_LEVEL:
				info = "[WARN]";
				break;
			case ERROR_LEVEL:
				info = "[ERROR]";
				break;
			case FATAL_LEVEL:
				info = "[FATAL]";
				break;
			default:
				info = "[FAIL]";
				break;
			}

			ImGui::TextColored(Settings::infoColor[entry.infoLevel],
							   "%s %s %s", entry.logTime.c_str(), info.c_str(),
							   entry.msg.c_str());

			//ImGui::TextUnformatted(str.c_str());
		}
		ImGui::EndChild();
		ImGui::End();
	}
}