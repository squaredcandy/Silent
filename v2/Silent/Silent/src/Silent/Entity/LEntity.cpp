#include "LEntity.h"
#include "../Event/EventDispatcher.h"

using namespace Silent;
Entity * LEntity::AddEntity(std::string name, Entity * parent,
							 glm::vec3 translation, glm::vec3 rotation, glm::vec3 scale)
{
	auto * e = new Entity();
	entities.emplace_back(e);

	EntityAddedEvent event;
	event.entity = e;
	event.name = name;
	event.parent = parent;
	event.transform[0] = translation;
	event.transform[1] = rotation;
	event.transform[2] = scale;
	EventDispatcher<EntityAddedEvent>::Dispatch(&event);
	return e;
}

void LEntity::Cleanup()
{
	for (auto& e : entities)
	{
		delete e;
	}
	entities.clear();
}

void LEntity::RemoveEntity(EntityID id)
{
	auto it = std::find_if(entities.begin(), entities.end(), [=] (Entity * e) 
	{
		return e->_entityID == id;
	});
	EntityRemovedEvent event;
	event.entity = *it;
	EventDispatcher<EntityRemovedEvent>::Dispatch(&event);
	if (it != entities.end())
	{
		entities.erase(it);
	}
}

void LEntity::OnInitialize()
{
}

void LEntity::OnUpdate()
{
}

void LEntity::OnDestroy()
{
	Cleanup();
}

std::vector<EntityID> LEntity::EntitiesThatMatch(std::bitset<MAX_COMPONENTS> key)
{
	std::vector<EntityID> ids;
	for (const auto& e : entities)
	{
		if ((e->components & key) == key)
		{
			ids.emplace_back(e->_entityID);
		}
	}
	return ids;
}
