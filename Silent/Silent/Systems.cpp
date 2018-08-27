#include "Systems.h"

namespace Silent::Engine::Systems
{
	bool updateEntities;
	std::vector<std::unique_ptr<System>> systems;

	void Update()
	{
		if (updateEntities)
		{
			for (auto& s : systems)
			{
				s->UpdateEntities();
			}
			updateEntities = false;
		}
	}

	void Execute()
	{
		for (auto& s : systems)
		{
			if (s->enabled)
			{
				s->Execute();
			}
		}
	}
}