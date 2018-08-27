#pragma once

#include "System.h"

namespace Silent::Engine::Systems
{
	// Instead of finding the entites we need to update every tick
	// we update only when modules are added or removed
	extern bool updateEntities;
	extern std::vector<std::unique_ptr<System>> systems;

	template <typename T, typename... TArgs> T& AddSystem(TArgs&&... mArgs)
	{
		T* c(new T(std::forward<TArgs>(mArgs)...));
		c->enabled = true;
		std::unique_ptr<System> uPtr{ c };
		systems.emplace_back(std::move(uPtr));

		updateEntities = true;
		return *c;
	}

	// Update the entites for each system
	void Update();

	// Execute system
	void Execute();


}