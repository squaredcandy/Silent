#include "Modules.h"
#include "Singleton.h"
#include "../Entity/Entities.h"

namespace Silent
{

	const std::map<std::type_index, std::pair<int, int>>& Modules::GetModifiedCounter()
	{
		return typesCounter;
	}

	void Modules::UpdateTypesModified()
	{
		if (!typesHandled.empty())
		{
			for (const auto& [key, val] : typesHandled)
			{
				typesModified[key] = false;
				typesCounter[key] = { 0, 0 };
			}
			typesHandled.clear();
		}
	}

// 	MapTypeToConAModule Modules::GetModules(std::vector<std::type_index> types, 
// 											bool onlyActive)
// 	{
// 		MapTypeToConAModule map;
// 
// 		auto entities = Singleton<Entities>::Instance().GetEntities();
// 		
// 		std::vector<EntityID> ids;
// 		ids.reserve(entities.size() / 2);
// 
// 		std::sort(types.begin(), types.end());
// 		
// 		// Get all valid ID's
// 		for (const auto& entity : entities)
// 		{
// 			auto mods = entity->modules;
// 			//std::sort(mods.begin(), mods.end());
// 			if (std::includes(types.begin(), types.end(), 
// 							  mods.begin(), mods.end()))
// 			{
// 				ids.emplace_back(entity->_entityID);
// 			}
// 		}
// 
// 		// Get all the modules 
// 		for (const auto& type : types)
// 		{
// 			for (const auto& mod : modules[type])
// 			{
// 				if (std::find(ids.begin(), ids.end(), 
// 							  mod->_entity->_entityID) != ids.end())
// 				{
// 					map[type].emplace(mod.get());
// 				}
// 			}
// 		}
// 
// 		return map;
// 	}

// 	MapTypeToConAModule Modules::GetModulesAddedThisFrame(
// 		std::vector<std::type_index> types, bool onlyActive)
// 	{
// 		MapTypeToConAModule map;
// 
// 		for (const auto& type : types)
// 		{
// 			auto sizeDiff = modules[type].size() - typesCounter[type].first;
// 
// 			for (auto it = std::next(modules[type].begin(), sizeDiff);
// 				 it != modules[type].end(); ++it)
// 			{
// 				map[type].emplace((*it).get());
// 			}
// 		}
// 
// 		return map;
// 	}
}