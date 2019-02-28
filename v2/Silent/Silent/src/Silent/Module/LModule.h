#pragma once

#include "Module.h"
#include "../Entity/LEntity.h"
#include "../Layer/Layer.h"
#include "../Utility/Locator.h"

#include <set>
#include <typeindex>
#include <typeinfo>

// template<typename T>
// std::pair<T, bool> GetSetElement(std::set<T>& searchSet, int i)
// {
// 	std::pair<T, bool> result;
// 	if (searchSet.size() > i)
// 	{
// 		result.first = *(std::next(searchSet.begin(), i));
// 		result.second = true;
// 	}
// 	else
// 	{
// 		result.second = false;
// 	}
// 	return result;
// }
// 
// namespace Silent
// {
// 	// Various Module Ptr
// 	using SModule = std::shared_ptr<Module>;
// 
// 	// Selected Module Ptr
// 	using ModulePtr = SModule;
// 
// 	struct ModulePtrComparitor
// 	{
// 		bool operator()(const ModulePtr& l, const ModulePtr& r) const
// 		{
// 			return l->_entity->_entityID < r->_entity->_entityID;
// 		}
// 	};
// 
// 	// Container of module Ptr's
// 	using ConModulePtr = std::set<ModulePtr, ModulePtrComparitor>;
// 
// 	// Mapping the type index to a container module ptr
// 	using MapTypeToConModulePtr = std::map<std::type_index, ConModulePtr>;
// 
// 	//struct ModuleTypeComp
// 	//{
// 	//	bool operator()(const std::type_index& a, const ModuleReference& b)
// 	//	{
// 	//		return a < b.t;
// 	//	}
// 	//};
// 
// 	// This is meant to be used with the singleton pattern
// 	class SILENT_API LModule
// 	{
// 	private:
// 		// y we have do this idk
// 		ModulePtr b;
// 		MapTypeToConModulePtr modules;
// 
// 		// check if the modules have been modified so we can update them in systems
// 		std::map<std::type_index, bool> typesModified;
// 
// 		// Tally the modules types that have been modified 
// 		// pair<added, removed>
// 		std::map<std::type_index, std::pair<int, int>> typesCounter;
// 
// 		// The types that were handled this frame
// 		std::map<std::type_index, bool> typesHandled;
// 
// 	public:
// 		template<typename T> inline bool AddModuleType()
// 		{
// 			ConModulePtr v;
// 			// We do this so we can get back a bool to check if it was successful
// 			auto success = modules.emplace(typeid(T), std::move(v));
// 			typesModified[typeid(T)] = false;
// 			return success.second;
// 		}
// 
// 		template<typename T>
// 		bool TypeModified()
// 		{
// 			if (typesModified[typeid(T)])
// 			{
// 				typesHandled[typeid(T)] = true;
// 				return true;
// 			}
// 			return false;
// 		}
// 
// 		template<typename T, typename S, typename... Arg>
// 		bool TypeModified()
// 		{
// 			if (typesModified[typeid(T)])
// 			{
// 				typesHandled[typeid(T)] = true;
// 			}
// 
// 			return TypeModified<S, Arg...>() || typesModified[typeid(T)];
// 		}
// 
// 		const std::map<std::type_index, std::pair<int, int>>& GetModifiedCounter();
// 
// 		void UpdateTypesModified();
// 
// 		// Force the systems that use this module to update
// 		template<typename T>
// 		void ForceUpdate()
// 		{
// 			typesModified[typeid(T)] = true;
// 		}
// 
// 		template<typename T, typename S, typename... Arg>
// 		void ForceUpdate()
// 		{
// 			ForceUpdate<S, Arg...>();
// 			typesModified[typeid(T)] = true;
// 		}
// 
// 		template<typename T, typename... TArgs>
// 		T* AddModule(std::shared_ptr<Entity> entity, TArgs&&... mArgs)
// 		{
// 			// Do we have the module?
// 			if (!modules.count(typeid(T)))
// 			{
// 				// Create the module
// 				if (!AddModuleType<T>())
// 				{
// 					// return false if we cant create one
// 					return nullptr;
// 				}
// 			}
// 
// 			// Add the module
// 			T* m(new T(std::forward<TArgs>(mArgs)...));
// 			m->_entity = entity;
// 			ModulePtr uPtr{ m };
// 			auto success = modules[typeid(T)].insert(std::move(uPtr));
// 			if (success.second)
// 			{
// 				typesModified[typeid(T)] = true;
// 				typesCounter[typeid(T)].first++;
// 
// 				// Add a reference to it in the entity for quick query and retrival
// 				auto pos = entity->modTypes.emplace(typeid(T));
// 				if (pos.second)
// 				{
// 					auto dist = std::distance(entity->modTypes.begin(), pos.first);
// 					entity->mods.emplace(std::next(entity->mods.begin(), dist),
// 										 success.first->get());
// 				}
// 				return dynamic_cast<T*>(success.first->get());
// 			}
// 			return nullptr;
// 		}
// 
// 		template<typename T>
// 		T* GetModule(std::shared_ptr<Entity> entity)
// 		{
// 			// Do we have the module?
// 			if (!modules.count(typeid(T)))
// 			{
// 				return nullptr;
// 			}
// 
// 			const auto& mods = modules[typeid(T)];
// 			for (const auto& mod : mods)
// 			{
// 				if (mod->_entity == entity)
// 				{
// 					return dynamic_cast<T*>(mod.get());
// 				}
// 			}
// 			return nullptr;
// 		}
// 
// 		template<typename T>
// 		bool RemoveModule(std::shared_ptr<Entity> entity)
// 		{
// 			for (auto& mod : modules[typeid(T)])
// 			{
// 				if (mod->_entity == entity)
// 				{
// 					mod->_entity->modTypes.erase(typeid(T));
// 					//mod->_entity->mods.erase(mod);
// 					modules[typeid(T)].erase(mod);
// 					typesModified[typeid(T)] = true;
// 					typesCounter[typeid(T)].second++;
// 					return true;
// 				}
// 			}
// 			return false;
// 		}
// 
// 		template<typename T>
// 		bool WasAModuleRemovedThisFrame(bool onlyActive = true)
// 		{
// 			return typesCounter[typeid(T)].second != 0;
// 		}
// 
// 		template<typename T, typename S, typename... Arg>
// 		bool WasAModuleRemovedThisFrame(bool onlyActive = true)
// 		{
// 			return WasAModuleRemovedThisFrame<T>(onlyActive) ||
// 				WasAModuleRemovedThisFrame<S, Arg...>(onlyActive);
// 		}
// 
// 		template<typename T>
// 		inline MapTypeToConModulePtr GetModulesAddedThisFrame(bool onlyActive = true)
// 		{
// 			MapTypeToConModulePtr map;
// 
// 			auto sizeDiff = modules[typeid(T)].size() - typesCounter[typeid(T)].first;
// 
// 			for (auto it = std::next(modules[typeid(T)].begin(), sizeDiff);
// 				 it != modules[typeid(T)].end(); ++it)
// 			{
// 				map[typeid(T)].emplace((*it));
// 			}
// 
// 			return map;
// 		}
// 
// 		template<typename T, typename S, typename... Arg>
// 		inline MapTypeToConModulePtr GetModulesAddedThisFrame(bool onlyActive = true)
// 		{
// 			static std::set<std::type_index> types{ typeid(T), typeid(S), typeid(Arg)... };
// 			MapTypeToConModulePtr map;
// 
// 			for (const auto& type : types)
// 			{
// 				auto sizeDiff = modules[type].size() - typesCounter[type].first;
// 
// 				for (auto it = std::next(modules[type].begin(), sizeDiff);
// 					 it != modules[type].end(); ++it)
// 				{
// 					map[type].emplace((*it));
// 				}
// 			}
// 
// 			return map;
// 		}
// 
// 		template<typename T>
// 		inline MapTypeToConModulePtr GetModules(bool useFilter = true,
// 												bool onlyActive = true)
// 		{
// 			MapTypeToConModulePtr map;
// 			MapTypeToConModulePtr filter;
// 			MapTypeToConModulePtr * modPtr = &modules;
// 			if (useFilter)
// 			{
// 				filter = GetModulesAddedThisFrame<T>(onlyActive);
// 				modPtr = &filter;
// 			}
// 
// 			// find the module type
// 			if (!modPtr->count(typeid(T))) return map;
// 
// 			for (auto& m : (*modPtr)[typeid(T)])
// 			{
// 				if (onlyActive && !m->_entity->_active) continue;
// 				map[typeid(T)].emplace(m);
// 			}
// 
// 			return map;
// 		}
// 
// 		template<typename T, typename S, typename... Arg>
// 		inline MapTypeToConModulePtr GetModules(bool useFilter = true,
// 												bool onlyActive = true)
// 		{
// 			static std::set<std::type_index> types{ typeid(T),
// 				typeid(S), typeid(Arg)... };
// 
// 			MapTypeToConModulePtr map;
// 			MapTypeToConModulePtr filter;
// 			MapTypeToConModulePtr * modPtr = &modules;
// 			if (useFilter)
// 			{
// 				filter = GetModulesAddedThisFrame<T, S, Arg...>(onlyActive);
// 				modPtr = &filter;
// 			}
// 
// 			for (const auto& type : types) if (!modPtr->count(type)) return map;
// 
// 			const auto& entities = Singleton<LEntity>::Instance().GetEntities();
// 
// 			std::set<EntityID> ids;
// 			// Get all valid ID's
// 			for (const auto& entity : entities)
// 			{
// 				const auto& mods = entity->modTypes;
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
// 				for (const auto& mod : (*modPtr)[type])
// 				{
// 					if (std::find(ids.begin(), ids.end(),
// 								  mod->_entity->_entityID) != ids.end())
// 					{
// 						map[type].emplace(mod);
// 					}
// 				}
// 			}
// 
// 			return map;
// 		}
// 	};
// }
 
namespace Silent
{
	class SILENT_API LModule : public Layer
	{
	private:
		std::map<ModuleID, std::vector<Module *>> modules;
		std::vector<ModuleID> updatedModules;

		void AddModuleCallback(Entity * entity, Module * module);
		void RemoveModuleCallback(Entity * entity, Module * module);

		template<typename T>
		bool ModuleModified();
		template<typename T, typename S, typename... Arg>
		bool ModuleModified();
	public:
		LModule() = default;
		~LModule() = default;

		template<typename T, typename... TArgs> 
		Module * AddModule(Entity * entity, TArgs&&... mArgs);

		template<typename T>
		void RemoveModule(Entity * entity);

		template<typename T>
		Module * GetModule(Entity * entity);

		template<typename T>
		std::vector<Module *> GetModules();

		template<typename T, typename S, typename... Arg>
		std::vector<std::tuple<T*, S*, Arg*...>>& GetModules();

		template<typename T>
		void ForeachModule(std::function<void(T*)> fn);

		inline void OnInitialize() override;
		inline void OnUpdate() override;
		inline void OnDestroy() override;
		inline std::vector<Module *>::iterator FindModule(ModuleID modID, Entity * entity);
		inline std::vector<Module *>::iterator FindModule(ModuleID modID, EntityID id);
	};

	template<typename T>
	bool LModule::ModuleModified()
	{
		return (std::find(updatedModules.begin(), updatedModules.end(), GetModuleTypeID<T>()) != updatedModules.end());
	}
	
	template<typename T, typename S, typename... Arg>
	bool LModule::ModuleModified()
	{
		return (ModuleModified<T>() || ModuleModified<S, Arg...>());
	}

	template<typename T, typename... TArgs>
	Module * LModule::AddModule(Entity * entity, TArgs&&... mArgs)
	{
		ModuleID modID = GetModuleTypeID<T>();
		T * newMod{ new T(std::forward<TArgs>(mArgs)...) };
		newMod->_entityID = entity->_entityID;
		entity->AddComponent(modID);
		modules[modID].emplace_back(newMod);

		AddModuleCallback(entity, newMod);
		return newMod;
	}

	template<typename T>
	void LModule::RemoveModule(Entity * entity)
	{
		ModuleID modID = GetModuleTypeID<T>();
		if (entity->HasComponent(modID))
		{
			auto it = FindModule(modID, entity);

			RemoveModuleCallback(entity, *it);

			entity->RemoveComponent(modID);
			modules[modID].erase(it);
		}
	}

	template<typename T>
	Module * LModule::GetModule(Entity * entity)
	{
		ModuleID modID = GetModuleTypeID<T>();
		if (!entity->HasComponent(modID))
		{
			// throw error?
			return nullptr;
		}
		return *FindModule(modID, entity);

	}

	template<typename T>
	std::vector<Module *> LModule::GetModules()
	{
		ModuleID modID = GetModuleTypeID<T>();
		return modules[modID];
	}

	template<typename T>
	void LModule::ForeachModule(std::function<void(T*)> fn)
	{
		ModuleID modID = GetModuleTypeID<T>();
		std::for_each(modules[modID].begin(), modules[modID].end(), fn);
	}

	template<typename T, typename S, typename... Arg, std::size_t Size, std::size_t... Argsize>
	auto ArrayToTupleHelper(const std::array<Module*, Size>& a, std::index_sequence<Argsize...>)
	{
		return std::make_tuple(dynamic_cast<T*>(a[0]), dynamic_cast<S*>(a[1]),
							   dynamic_cast<Arg*...>(a[2 + Argsize])...);
	}

	template<std::size_t Size, std::size_t Argsize, typename T, typename S, typename... Arg>
	auto ArrayToTuple(const std::array<Module*, Size>& a)
	{
		return ArrayToTupleHelper<T, S, Arg...>(a, std::make_index_sequence<Argsize>{});
	}
	
	template<typename T, typename S, typename... Arg>
	std::vector<std::tuple<T *, S *, Arg *...>>& LModule::GetModules()
	{
		static auto entities = Locator<LEntity>::Get();
		static std::vector<std::tuple<T *, S *, Arg *...>> result;
		
		if (ModuleModified<T, S, Arg...>())
		{
			result.clear();
		}
		else
		{
			static bool firstTime = true;
			if (!result.empty() || !firstTime)
			{
				firstTime = false;
				return result;
			}
		}

		constexpr std::size_t Argsize = sizeof...(Arg);
		constexpr std::size_t size = Argsize + 2;

		std::array<ModuleID, size> moduleIDs
		{ GetModuleTypeID<T>(), GetModuleTypeID<S>(), GetModuleTypeID<Arg>()... };

		std::bitset<MAX_COMPONENTS> key;
		for (const auto& id : moduleIDs)
		{
			key[id] = true;
		}

		std::vector<EntityID> matches = entities->EntitiesThatMatch(key);
		if (matches.empty()) return result;
		result.reserve(matches.size());
		std::array<Module *, size> moduleStack;

		for (const auto& entityID : matches)
		{
			for (int j = 0; j < size; ++j)
			{
				moduleStack[j] = *FindModule(moduleIDs[j], entityID);
			}
			result.emplace_back(ArrayToTuple<size, Argsize, T, S, Arg...>(moduleStack));
		}

		return result;
	}
}