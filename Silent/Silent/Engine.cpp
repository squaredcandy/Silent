#include "Engine.h"

#include <any>

namespace Silent::Engine
{
	std::shared_ptr<Backend> backend;

	void InitEngine()
	{
#if defined BACKEND_SDL_GL
		backend = std::make_shared<BackendSDLGL>();
#elif defined BACKEND_DEFAULT
		backend = std::make_shared<BackendSDLGL>();
#endif // BACKEND_SDL_GL

		backend->Init();
		backend->SettingsInit();
	}

	void GameLoop()
	{
		bool done = false;
		while (!done)
		{
			done = backend->UpdateEvent();
			backend->Update();

			Systems::Update();
			Systems::Execute();

			bool a = true;
			ImGui::ShowDemoWindow(&a);

			ImGui::Begin("test", &a);
			if (ImGui::CollapsingHeader("Entity"))
			{
				auto ent = Entities::GetAll();
				static std::string currentEntityName{ "" };
				static EntityID currentID = -1;
				static Entity* currentEntity;
				if (ImGui::BeginCombo("##Entity", currentEntityName.c_str()))
				{
					for (auto& e : ent)
					{
						std::string entityName = e->GetCommon().name;
						EntityID id = e->GetID();
						bool selected = (currentID = id);
						ImGui::PushID((int) id);
						if (ImGui::Selectable(entityName.c_str(), selected))
						{
							currentEntityName = entityName;
							currentID = id;
							currentEntity = e;
						}
						if (selected)
						{
							ImGui::SetItemDefaultFocus();
						}
						ImGui::PopID();
					}
					ImGui::EndCombo();
				}
				if (currentEntity)
				{
					currentEntity->GUI();
				}
			}
			ImGui::End();

 			backend->Render();
		}
	}

	void Cleanup()
	{
		Resources::CleanupResources();
		backend->Cleanup();
	}
}