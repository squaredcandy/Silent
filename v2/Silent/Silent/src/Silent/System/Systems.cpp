#include "Systems.h"

#include "../Utility/Singleton.h"
#include "../Module/Modules.h"

namespace Silent
{
	void Systems::UpdateSystemModules()
	{
		static auto& modules = Singleton<Modules>::Instance();
		for (auto& sys : systems)
		{
			sys->UpdateEntities(modules);
		}
		modules.UpdateTypesModified();
	}

	void Systems::RunSystems()
	{
		UpdateSystemModules();
		for (auto& sys : systems)
		{
			sys->Execute();
		}
	}

	void Systems::Cleanup()
	{
		for (const auto& sys : systems)
		{
			sys->Cleanup();
		}
		systems.clear();
	}

}