#pragma once

#include "Module.h"
#include "../Utility/Singleton.h"

#include "../Entity/Entities.h"

#include <set>
#include <typeindex>
#include <typeinfo>

template<typename T>
std::pair<T, bool> GetSetElement(std::set<T>& searchSet, int i)
{
	std::pair<T, bool> result;
	if (searchSet.size() > i)
	{
		result.first = *(std::next(searchSet.begin(), i));
		result.second = true;
	}
	else
	{
		result.second = false;
	}
	return result;
}

namespace Silent
{
	// Various Module Ptr
	using UModule = std::unique_ptr<Module>;
	using SModule = std::shared_ptr<Module>;
	using AModule = Module *;

	// Selected Module Ptr
	using ModulePtr = UModule;

	struct ModulePtrComparitor
	{
		bool operator()(const ModulePtr& l, const ModulePtr& r) const
		{
			return l->_entity->_entityID < r->_entity->_entityID;
		}
	};

	struct AModuleComparitor
	{
		bool operator()(const AModule& l, const AModule& r) const
		{
			return l->_entity->_entityID < r->_entity->_entityID;
		}
	};

	// Container of module Ptr's
	using ConModulePtr = std::set<ModulePtr, ModulePtrComparitor>;

	// Container of Module * - Used for returns
	using ConAModule = std::set<AModule, AModuleComparitor>;
	
	// Mapping the type index to a container module ptr
	using MapTypeToConModulePtr = std::map<std::type_index, ConModulePtr>;

	// Mapping the type index to a container module ptr
	using MapTypeToConAModule = std::map<std::type_index, ConAModule>;

	// This is meant to be used with the singleton pattern
	class SILENT_API Modules
	{
	private:
		//const int MODULES_SIZE = 100;
		// y we have do this idk
		ModulePtr b;
		MapTypeToConModulePtr modules;

		// check if the modules have been modified so we can update them in systems
		std::map<std::type_index, bool> typesModified;

		// Tally the modules types that have been modified 
		// pair<added, removed>
		std::map<std::type_index, std::pair<int, int>> typesCounter;

		// The types that were handled this frame
		std::map<std::type_index, bool> typesHandled;

	public:
		template<typename T> inline bool AddModuleType()
		{
			ConModulePtr v;
			// We do this so we can get back a bool to check if it was successful
			auto success = modules.emplace(typeid(T), std::move(v));
			typesModified[typeid(T)] = false;
			return success.second;
		}

		template<typename T>
		bool TypeModified()
		{
			if (typesModified[typeid(T)])
			{
				typesHandled[typeid(T)] = true;
				return true;
			}
			return false;
		}

		template<typename T, typename S, typename... Arg>
		bool TypeModified()
		{
			if (typesModified[typeid(T)])
			{
				typesHandled[typeid(T)] = true;
			}

			return TypeModified<S, Arg...>() || typesModified[typeid(T)];
		}

		const std::map<std::type_index, std::pair<int, int>>& GetModifiedCounter();

		void UpdateTypesModified();

		// Force the systems that use this module to update
		template<typename T>
		void ForceUpdate()
		{
			typesModified[typeid(T)] = true;
		}

		template<typename T, typename S, typename... Arg>
		void ForceUpdate()
		{
			ForceUpdate<S, Arg...>();
			typesModified[typeid(T)] = true;
		}

		template<typename T, typename... TArgs>
		T* AddModule(std::shared_ptr<Entity> entity, TArgs&&... mArgs)
		{
			// Do we have the module?
			if (!modules.count(typeid(T)))
			{
				// Create the module
				if (!AddModuleType<T>())
				{
					// return false if we cant create one
					return nullptr;
				}
			}

			// Add the module
			T* m(new T(std::forward<TArgs>(mArgs)...));
			m->_entity = entity;
			ModulePtr uPtr{ m };
			auto success = modules[typeid(T)].insert(std::move(uPtr));
			typesModified[typeid(T)] = true;
			typesCounter[typeid(T)].first++;

			// Add a reference to it in the entity for quick query and retrival
			entity->modules.emplace(typeid(T));

			return dynamic_cast<T*>(success.first->get());
		}

		template<typename T>
		T* GetModule(std::shared_ptr<Entity> entity)
		{
			// Do we have the module?
			if (!modules.count(typeid(T)))
			{
				return nullptr;
			}

			const auto& mods = modules[typeid(T)];
			for (const auto& mod : mods)
			{
				if (mod->_entity == entity)
				{
					return dynamic_cast<T*>(mod.get());
				}
			}
			return nullptr;
		}

		template<typename T>
		bool RemoveModule(std::shared_ptr<Entity> entity)
		{
			for (auto& mod : modules[typeid(T)])
			{
				if (mod->_entity == entity)
				{
					mod->_entity->modules.erase(typeid(T));
;					modules[typeid(T)].erase(mod);
					typesModified[typeid(T)] = true;
					typesCounter[typeid(T)].second++;
					return true;
				}
			}
			return false;
		}

		template<typename T>
		MapTypeToConAModule GetModulesAddedThisFrame(bool onlyActive)
		{
			MapTypeToConAModule map;

			auto sizeDiff = modules[typeid(T)].size() - typesCounter[typeid(T)].first;

			for (auto it = std::next(modules[typeid(T)].begin(), sizeDiff);
				 it != modules[typeid(T)].end(); ++it)
			{
				map[typeid(T)].emplace((*it).get());
			}

			return map;
		}

		template<typename T, typename S, typename... Arg>
		MapTypeToConAModule GetModulesAddedThisFrame(bool onlyActive)
		{
			static std::set<std::type_index> types{ typeid(T), typeid(S), typeid(Arg)... };
			MapTypeToConAModule map;

			for (const auto& type : types)
			{
				auto sizeDiff = modules[type].size() - typesCounter[type].first;

				for (auto it = std::next(modules[type].begin(), sizeDiff);
					 it != modules[type].end(); ++it)
				{
					map[type].emplace((*it).get());
				}
			}

			return map;
		}

		template<typename T>
		MapTypeToConAModule GetModulesFiltered(MapTypeToConAModule& filter,
											   bool onlyActive = true)
		{
			MapTypeToConAModule map;

			// find the module type
			if (!filter.count(typeid(T))) return map;

			for (auto& m : filter[typeid(T)])
			{
				if (onlyActive && !m->_entity->_active) continue;
				map[typeid(T)].emplace(m);
			}

			return map;
		}

		// Template version of Get Modules
		// Get all the modules of one type
		template<typename T>
		MapTypeToConAModule GetModulesUnfiltered(bool onlyActive)
		{
			MapTypeToConAModule map;

			// find the module type
			if (!modules.count(typeid(T))) return map;

			for (auto& m : modules[typeid(T)])
			{
				if (onlyActive && !m->_entity->_active) continue;
				map[typeid(T)].emplace(m.get());
			}

			return map;
		}

		template<typename T, typename S, typename... Arg>
		MapTypeToConAModule GetModulesFiltered(MapTypeToConAModule& filter, 
											   bool onlyActive = true)
		{
			MapTypeToConAModule map;
			static std::set<std::type_index> types{ typeid(T), typeid(S), typeid(Arg)... };
			for (const auto& type : types) if (!filter.count(type)) return map;

			const auto& entities = Singleton<Entities>::Instance().GetEntities();
			std::set<EntityID> ids;

			// Get all valid ID's
			for (const auto& entity : entities)
			{
				const auto& mods = entity->modules;
				//std::sort(mods.begin(), mods.end());
				if (std::includes(types.begin(), types.end(),
								  mods.begin(), mods.end()))
				{
					ids.emplace(entity->_entityID);
				}
			}

			// Get all the modules 
			for (const auto& type : types)
			{
				for (const auto& mod : filter[type])
				{
					if (std::find(ids.begin(), ids.end(),
								  mod->_entity->_entityID) != ids.end())
					{
						map[type].emplace(mod);
					}
				}
			}

			return map;
		}

		template<typename T, typename S, typename... Arg>
		MapTypeToConAModule GetModulesUnfiltered(bool onlyActive)
		{
			MapTypeToConAModule map;
			static std::set<std::type_index> types { typeid(T), typeid(S), typeid(Arg)... };
			for (const auto& type : types)
			{
				if (modules.count(type) == 0) return map;
			}
			
			const auto& entities = Singleton<Entities>::Instance().GetEntities();
			std::set<EntityID> ids;

			// Get all valid ID's
			for (const auto& entity : entities)
			{
				const auto& mods = entity->modules;
				//std::sort(mods.begin(), mods.end());
				if (std::includes(types.begin(), types.end(),
								  mods.begin(), mods.end()))
				{
					ids.emplace(entity->_entityID);
				}
			}

			// Get all the modules 
			for (const auto& type : types)
			{
				for (const auto& mod : modules[type])
				{
					if (std::find(ids.begin(), ids.end(),
								  mod->_entity->_entityID) != ids.end())
					{
						map[type].emplace(mod.get());
					}
				}
			}

			return map;
		}
	};
}