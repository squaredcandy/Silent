#include "nlohmann/json.hpp"
using namespace nlohmann;

#include <iostream>

#include "Engine.h"
#include "TestSystem.h"
#include "Log.h"

#define IMGUI_NEWWINDOW ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver)

using namespace Silent::Engine;


void LoadResources()
{
	//Resources::LoadResource<Mesh>("box", "box_export.FBX");
	Systems::AddSystem<TestSystem>();
	auto obj1 = Entities::AddEntity("Object 1");
	Entities::AddEntity("Object 2", obj1->GetChildren());
	
	//auto m = Resources::GetResource<Mesh>("box");
}

void Loop()
{

}

// Keep windows open
bool a = true;

static EntityID selectedID = UINT_MAX;

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

void ObjectInspector()
{
	IMGUI_NEWWINDOW;
	ImGui::Begin("Object Inspector", &a);
	
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

void Stats()
{
	static Uint32 lastTime = SDL_GetTicks();
	static Uint32 totalTime = SDL_GetTicks();
	static Uint32 deltaTime;

	lastTime = totalTime;
	totalTime = SDL_GetTicks();
	deltaTime = totalTime - lastTime;

	IMGUI_NEWWINDOW;
	ImGui::Begin("Stats", &a);
	ImGui::Text("Total Time: %.1fs", totalTime / 100.f);
	ImGui::Text("Delta Time: %ums", deltaTime);
	ImGui::End();
}

void ConfigureSettings()
{
	ImGui::Begin("Settings", &a);
	Silent::Settings::SettingsGUI();
	ImGui::End();
}

void Window()
{
	Silent::Engine::Log::Draw();

	ObjectInspector();

	Stats();

	ConfigureSettings();
}

int main(int argc, char ** argv)
{
	InitEngine();
	Log::InitLog();
	
	Resources::LoadAllResources();
	LoadResources();
	
	loopFunction = &Loop;
	windowFunction = &Window;

	GameLoop();
	Cleanup();
	return 0;
}