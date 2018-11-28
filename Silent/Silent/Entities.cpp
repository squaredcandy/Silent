#include "Entities.h"

namespace Silent::Engine::Entities
{
	void Update(float deltaTime, EntityContainer& entities)
	{
		// We do this because we can potentually add an entity during the update
		for (auto i = 0; i < entities.size(); ++i)
		{
			entities[i]->Update(deltaTime);

			Update(deltaTime, entities[i]->GetChildren());
			RemoveInactive(entities[i]->GetChildren());
		}
		RemoveInactive(entities);
	}

	void RemoveInactive(EntityContainer& entities)
	{
		// Maybe make a new entity that gets their inv and drops it
		entities.erase(std::remove_if(std::begin(entities), std::end(entities),
									  [] (const std::unique_ptr<Entity> &e)
		{
			return !e->isActive();
		}), entities.end());
	}

	void RemoveAll(EntityContainer& entities)
	{
		entities.clear();
	}

	std::size_t Size(EntityContainer& entities)
	{
		return entities.size();
	}

	Entity * AddEntity(std::string name, EntityContainer& entities,
					   glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
	{
		std::unique_ptr<Entity> uPtr = std::make_unique<Entity>();
		uPtr->AddMod<ModCommon>(name);
		uPtr->AddMod<ModTransform>(translate, rotate, scale);
		entities.emplace_back(std::move(uPtr));

		return &*entities.back();
	}

	Entity * Get(EntityID id, EntityContainer& entities) noexcept
	{
		auto found = std::find_if(std::begin(entities), std::end(entities),
								  [=] (const std::unique_ptr<Entity>& e)
		{
			return (e->GetID() == id);
		});
		if (found != std::end(entities))
		{
			return (*found).get();
		}

		// We cant find it in the top layer, we go downwards
		for (const auto& entity : entities)
		{
			return Get(id, entity->GetChildren());
		}

		return nullptr;
	}

	std::vector<Entity*> GetAll(EntityContainer& entities, bool recursive)
	{
		std::vector<Entity*> found;
		for (auto & entity : entities)
		{
			// Get this ptr
			found.emplace_back(entity.get());
			
			if (!recursive) continue;

			// Get the ptr of all children (RECURSIVE!!)
			auto children = GetAll(entity->GetChildren());
			if (!children.empty())
			{
				found.reserve(children.size());
				found.insert(found.end(), children.begin(), children.end());
			}
		}
		return found;
	}
}