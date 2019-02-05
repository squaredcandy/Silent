#include "SDebug.h"

namespace Silent
{
	void SDebug::ForceUpdateModules(Modules& modules)
	{
	}

	void SDebug::IncrementalUpdateModules(Modules& modules)
	{
	}

	void SDebug::Execute()
	{
		auto a = true;
		auto& systems = _systems->GetSystems();

		ImGui::ShowDemoWindow(&a);

		if (ImGui::Begin("System Debug", &a))
		{
			// Do some combo thingy
			static int selected = -1;
			static std::string selectedSystem;
			static System * currentSystem = nullptr;
			if (ImGui::BeginCombo("Systems", selectedSystem.c_str()))
			{
				int i = 0;
				// A nothing selection
				if (ImGui::Selectable("", selected == -1))
				{
					selected = -1;
					selectedSystem = "";
					currentSystem = nullptr;
				}

				for (auto& system : systems)
				{
					if (ImGui::Selectable(system->_name.c_str(), selected == i))
					{
						selected = i;
						selectedSystem = system->_name;
						currentSystem = system.get();
					}
					++i;
				}
				ImGui::EndCombo();
			}
			
			if (currentSystem != nullptr)
			{
				currentSystem->DebugInfo();
			}

		}
		ImGui::End();
	}

	void SDebug::Cleanup()
	{
	}
}