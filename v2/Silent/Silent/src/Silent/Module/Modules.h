#pragma once

#include "Module.h"
#include "../Utility/Singleton.h"

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
		const int MODULES_SIZE = 100;
		// y we have do this idk
		ModulePtr b;
		MapTypeToConModulePtr modules;

		// check if the modules have been modified so we can update them in systems
		std::map<std::type_index, bool> typesModified;

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
					modules[typeid(T)].erase(mod);
					typesModified[typeid(T)] = true;
					return true;
				}
			}
			return false;
		}

		// Get all the modules of one type
		template<typename T>
		MapTypeToConAModule GetModules(bool onlyActive)
		{
			MapTypeToConAModule found;

			// find the module type
			if (!modules.count(typeid(T))) return found;

			found[typeid(T)] = ConAModule();
			// Get the modules
			ConAModule& mods = found[typeid(T)];

			for (auto& m : modules[typeid(T)])
			{
				if (onlyActive && !m->_entity->_active) continue;
				mods.emplace(m.get());
			}

			return found;
		}

		template<typename T, typename S, typename... Arg>
		MapTypeToConAModule GetModules(bool onlyActive)
		{
			// not using auto so intellisense can work
			auto matchAgainst = GetModules<T>(onlyActive);
			auto found = GetModules<S, Arg...>(onlyActive);

			auto& matchSet = matchAgainst[typeid(T)];
			auto& foundSet = found[typeid(S)];

			std::vector<EntityID> matchingIDs;
			matchingIDs.reserve(foundSet.size());

			// Find all the Entity ID's that are in both sets
			for (auto& f : foundSet)
			{
				for (auto& m : matchSet)
				{
					if (m->_entity == f->_entity)
					{
						matchingIDs.emplace_back(f->_entity->_entityID);
						break;
					}
				}
			}

			// Check the entity should to be removed
			const auto removeModulesCheck = [&] (const AModule& mod) -> bool
			{
				return (std::find(matchingIDs.begin(), matchingIDs.end(),
								  mod->_entity->_entityID) == matchingIDs.end());
			};

			// Remove unneeded modules 
			const auto removeModules = [&] (ConAModule& moduleSet)
			{
				for (auto it = moduleSet.begin(); it != moduleSet.end();)
				{
					if (removeModulesCheck(*it)) it = moduleSet.erase(it);
					else ++it;
				}
			};

			// Remove both sets modules
			removeModules(matchSet);
			for (auto&[key, val] : found) removeModules(val);

			found.merge(matchAgainst);

			return found;
		}
	};
}