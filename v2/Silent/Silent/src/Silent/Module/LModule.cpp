#include "LModule.h"
#include "../Event/EventDispatcher.h"
//#include "Singleton.h"
//#include "../Entity/LEntity.h"

//namespace Silent
//{
//	const std::map<std::type_index, std::pair<int, int>>& 
//		LModule::GetModifiedCounter()
//	{
//		return typesCounter;
//	}
//
//	void LModule::UpdateTypesModified()
//	{
//		if (!typesHandled.empty())
//		{
//// 			for (const auto& [key, val] : typesHandled)
//// 			{
//// 				typesModified[key] = false;
//// 				typesCounter[key] = { 0, 0 };
//// 			}
//			typesHandled.clear();
//		}
//	}
//}

using namespace Silent;

void LModule::OnInitialize()
{
	auto addEnt = [&] (EntityAddedEvent& e)
	{
		AddModule<MTransform>(e.entity, e.name, e.parent, 
							  e.transform[0], e.transform[1], e.transform[2]);
		return false;
	};

	auto addMod = [&] (ModuleAddedEvent& e) 
	{
		updatedModules.emplace_back(e.module->_moduleID);
		return false;
	};

	auto removeMod = [&] (ModuleRemovedEvent& e)
	{
		updatedModules.emplace_back(e.module->_moduleID);
		return false;
	};

	EventDispatcher<EntityAddedEvent>::AddCallback(addEnt);
	EventDispatcher<ModuleAddedEvent>::AddCallback(addMod);
	EventDispatcher<ModuleRemovedEvent>::AddCallback(removeMod);
}

void LModule::OnUpdate()
{
	updatedModules.clear();
}

void LModule::OnDestroy()
{
	for (auto& [key, val] : modules)
	{
		for (auto& m : val)
		{
			delete m;
		}
	}

	updatedModules.clear();
	modules.clear();
}

std::vector<Module *>::iterator LModule::FindModule(ModuleID modID, Entity * entity)
{
	return std::find_if(modules[modID].begin(), modules[modID].end(),
						   [=] (Module * m)
	{
		return (m->_entityID == entity->_entityID);
	});
}

std::vector<Module *>::iterator LModule::FindModule(ModuleID modID, EntityID id)
{
	return std::find_if(modules[modID].begin(), modules[modID].end(),
						[=] (Module * m)
	{
		return (m->_entityID == id);
	});
}

void LModule::AddModuleCallback(Entity * entity, Module * module)
{
	ModuleAddedEvent event;
	event.entity = entity;
	event.module = module;
	EventDispatcher<ModuleAddedEvent>::Dispatch(&event);
}

void LModule::RemoveModuleCallback(Entity * entity, Module * module)
{
	ModuleRemovedEvent event;
	event.entity = entity;
	event.module = module;
	EventDispatcher<ModuleRemovedEvent>::Dispatch(&event);
}
