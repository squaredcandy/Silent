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
	using SModule = std::shared_ptr<Module>;

	// Selected Module Ptr
	using ModulePtr = SModule;

	struct ModulePtrComparitor
	{
		bool operator()(const ModulePtr& l, const ModulePtr& r) const
		{
			return l->_entity->_entityID < r->_entity->_entityID;
		}
	};

	// Container of module Ptr's
	using ConModulePtr = std::set<ModulePtr, ModulePtrComparitor>;

	// Mapping the type index to a container module ptr
	using MapTypeToConModulePtr = std::map<std::type_index, ConModulePtr>;

	//struct ModuleTypeComp
	//{
	//	bool operator()(const std::type_index& a, const ModuleReference& b)
	//	{
	//		return a < b.t;
	//	}
	//};

	// This is meant to be used with the singleton pattern
	class SILENT_API Modules
	{
	private:
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
			if (success.second)
			{
				typesModified[typeid(T)] = true;
				typesCounter[typeid(T)].first++;

				// Add a reference to it in the entity for quick query and retrival
				auto pos = entity->modTypes.emplace(typeid(T));
				if (pos.second)
				{
					auto dist = std::distance(entity->modTypes.begin(), pos.first);
					entity->mods.emplace(std::next(entity->mods.begin(), dist), 
										 success.first->get());
				}
				return dynamic_cast<T*>(success.first->get());
			}
			return nullptr;
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
		bool WasAModuleRemovedThisFrame(bool onlyActive = true)
		{
			return typesCounter[typeid(T)].second != 0;
		}

		template<typename T, typename S, typename... Arg>
		bool WasAModuleRemovedThisFrame(bool onlyActive = true)
		{
			return WasAModuleRemovedThisFrame<T>(onlyActive) || 
				WasAModuleRemovedThisFrame<S, Arg...>(onlyActive);
		}

		template<typename T>
		inline MapTypeToConModulePtr GetModulesAddedThisFrame(bool onlyActive = true)
		{
			MapTypeToConModulePtr map;

			auto sizeDiff = modules[typeid(T)].size() - typesCounter[typeid(T)].first;

			for (auto it = std::next(modules[typeid(T)].begin(), sizeDiff);
				 it != modules[typeid(T)].end(); ++it)
			{
				map[typeid(T)].emplace((*it));
			}

			return map;
		}

		template<typename T, typename S, typename... Arg>
		inline MapTypeToConModulePtr GetModulesAddedThisFrame(bool onlyActive = true)
		{
			static std::set<std::type_index> types{ typeid(T), typeid(S), typeid(Arg)... };
			MapTypeToConModulePtr map;

			for (const auto& type : types)
			{
				auto sizeDiff = modules[type].size() - typesCounter[type].first;

				for (auto it = std::next(modules[type].begin(), sizeDiff);
					 it != modules[type].end(); ++it)
				{
					map[type].emplace((*it));
				}
			}

			return map;
		}

		template<typename T>
		inline MapTypeToConModulePtr GetModules(bool useFilter = true,
												bool onlyActive = true)
		{
			MapTypeToConModulePtr map;
			MapTypeToConModulePtr filter;
			MapTypeToConModulePtr * modPtr = &modules;
			if (useFilter)
			{
				filter = GetModulesAddedThisFrame<T>(onlyActive);
				modPtr = &filter;
			}

			// find the module type
			if (!modPtr->count(typeid(T))) return map;

			for (auto& m : (*modPtr)[typeid(T)])
			{
				if (onlyActive && !m->_entity->_active) continue;
				map[typeid(T)].emplace(m);
			}

			return map;
		}

		template<typename T, typename S, typename... Arg>
		MapTypeToConModulePtr GetModules(bool useFilter = true, 
										 bool onlyActive = true)
		{
			static std::set<std::type_index> types{ typeid(T), 
				typeid(S), typeid(Arg)... };

			MapTypeToConModulePtr map;
			MapTypeToConModulePtr filter;
			MapTypeToConModulePtr * modPtr = &modules;
			if (useFilter)
			{
				filter = GetModulesAddedThisFrame<T, S, Arg...>(onlyActive);
				modPtr = &filter;
			}

			for (const auto& type : types) if (!modPtr->count(type)) return map;

			const auto& entities = Singleton<Entities>::Instance().GetEntities();

			std::set<EntityID> ids;
			// Get all valid ID's
			for (const auto& entity : entities)
			{
				const auto& mods = entity->modTypes;
				if (std::includes(mods.begin(), mods.end(),
								  types.begin(), types.end()))
				{
					ids.emplace(entity->_entityID);
				}
			}

			// Get all the modules 
			for (const auto& type : types)
			{
				for (const auto& mod : (*modPtr)[type])
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

// 		template<typename T, typename S, typename... Arg>
// 		inline MapTypeToConModulePtr GetModulesFiltered(bool onlyActive = true)
// 		{
// 			MapTypeToConModulePtr map;
// 			MapTypeToConModulePtr filter = GetModulesAddedThisFrame<T, S, Arg...>(onlyActive);
// 
// 			static std::set<ModuleReference> types{ ModuleReference(typeid(T)),
// 				ModuleReference(typeid(S)), ModuleReference(typeid(Arg))... };
// 			for (const auto& type : types) if (!filter.count(type.t)) return map;
// 
// 			const auto& entities = Singleton<Entities>::Instance().GetEntities();
// 			std::set<EntityID> ids;
// 
// 			// Get all valid ID's
// // 			for (const auto& entity : entities)
// // 			{
// // 				const auto& mods = entity->modules;
// // 				//std::sort(mods.begin(), mods.end());
// // 				if (std::includes(mods.begin(), mods.end(),
// // 								  types.begin(), types.end()))
// // 				{
// // 					//ids.emplace(entity->_entityID);
// // 					map[mods.t].emplace(mods.m);
// // 				}
// // 			}
// 
// 			std::set<ModuleReference> intersection;
// 			for (const auto& entity : entities)
// 			{
// 				const auto& mods = entity->modules;
// 				std::set_intersection(mods.begin(), mods.end(), 
// 									  types.begin(), types.end(), 
// 									  std::inserter(intersection, 
// 													intersection.begin()));
// 				if (intersection.size() == types.size())
// 				{
// 					for (auto& i : intersection)
// 					{
// 						map[i.t].emplace(i.m);
// 					}
// 				}
// 			}
// 
// 			// Get all the modules 
// // 			for (const auto& type : types)
// // 			{
// // 				for (const auto& mod : filter[type.t])
// // 				{
// // 					if (std::find(ids.begin(), ids.end(),
// // 								  mod->_entity->_entityID) != ids.end())
// // 					{
// // 						map[type.t].emplace(mod);
// // 					}
// // 				}
// // 			}
// 
// 			return map;
// 		}
// 
// 		template<typename T, typename S, typename... Arg>
// 		MapTypeToConModulePtr GetModulesUnfiltered(bool onlyActive = true)
// 		{
// 			MapTypeToConModulePtr map;
// 			static std::set<ModuleReference> types { ModuleReference(typeid(T)), 
// 				ModuleReference(typeid(S)), ModuleReference(typeid(Arg))... };
// 			for (const auto& type : types)
// 			{
// 				if (modules.count(type.t) == 0) return map;
// 			}
// 			
// 			const auto& entities = Singleton<Entities>::Instance().GetEntities();
// 			std::set<EntityID> ids;
// 
// 			// Get all valid ID's
// 			for (const auto& entity : entities)
// 			{
// 				const auto& mods = entity->modules;
// 				if (std::includes(mods.begin(), mods.end(),
// 								  types.begin(), types.end()))
// 				{
// 					ids.emplace(entity->_entityID);
// 				}
// 			}
// 
// 			// Get all the modules 
// 			for (const auto& type : types)
// 			{
// 				for (const auto& mod : modules[type.t])
// 				{
// 					if (std::find(ids.begin(), ids.end(),
// 								  mod->_entity->_entityID) != ids.end())
// 					{
// 						map[type.t].emplace(mod.get());
// 					}
// 				}
// 			}
// 
// 			return map;
// 		}
	};
}