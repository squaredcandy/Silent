#pragma once

#include "../Core.h"
#include "System.h"

#include <set>

namespace Silent
{
	// Class to be used with singleton
	class SILENT_API Systems
	{
	private:
		std::unique_ptr<System> a;
		std::set<std::unique_ptr<System>> systems;

		void UpdateSystemModules();
	public:
		void RunSystems();

		template<typename T, typename... TArgs>
		T* AddSystem(TArgs&&... mArgs)
		{
			T* s(new T(std::forward<TArgs>(mArgs)...));
			std::unique_ptr<System> uPtr{ s };
			auto success = systems.emplace(std::move(uPtr));
			if (success.second) return dynamic_cast<T*>(success.first->get());
			else return nullptr;
		}

		std::set<std::unique_ptr<System>>& GetSystems() { return systems; }

		void Cleanup();
	};
}