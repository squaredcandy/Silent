#pragma once

#include "Entity.h"
#include "Mods.h"
#include "Scene.h"

#include <vector>
#include <memory>

using EntityContainer = std::vector<std::unique_ptr<Entity>>;

namespace Silent::Engine::Scene
{
	extern EntityContainer entities;
}

// So these functions are kinda wierd
namespace Silent::Engine::Entities
{
	void Update(float deltaTime, EntityContainer& entities/* = Engine::currentScene.entities*/);
	void RemoveInactive(EntityContainer& entities/* = currentScene.entities*/);
	void RemoveAll(EntityContainer& entities/* = currentScene.entities*/);
	std::size_t Size(EntityContainer& entities/* = currentScene.entities*/);
	Entity * AddEntity(std::string name = "",
					   EntityContainer& entities = Scene::entities,
					   glm::vec3 translate = glm::vec3(0, 5, 0),
					   glm::vec3 rotate = glm::vec3(),
					   glm::vec3 scale = glm::vec3(1, 1, 1));
	Entity * Get(EntityID id, EntityContainer& entities = Scene::entities) noexcept;
	std::vector<Entity*> GetAll(EntityContainer& entities = Scene::entities, bool recursive = false);

	template<typename T> std::vector<Entity*> GetAll(
		EntityContainer& entities = Scene::entities, bool recursive = false)
	{
		std::vector<Entity*> found;
		for (auto & entity : entities)
		{
			if (entity->HasMod<T>())
			{
				found.emplace_back(entity.get());

				if (!recursive) continue;

				// Get the ptr of all children (RECURSIVE!!)
				auto children = GetAll<T>(entity->GetChildren());
				if (!children.empty())
				{
					found.reserve(children.size());
					found.insert(found.end(), children.begin(), children.end());
				}
			}
		}
		return found;
	}

	template<typename T, typename S, typename... Arg> 
	std::vector<Entity*> GetAll(
		EntityContainer& entities = Scene::entities, bool recursive = false)
	{
		std::vector<Entity*> found;
		std::vector<Entity*> total;
		for (auto & entity : entities)
		{
			if (entity->HasMod<T>())
			{
				found.emplace_back(entity);

				if (!recursive) continue;

				// Get the ptr of all children (RECURSIVE!!)
				auto children = GetAll<S, Arg...>(entity->GetChildren());
				if (!children.empty())
				{
					found.reserve(children.size());
					found.insert(found.end(), children.begin(), children.end());
				}
			}
		}
		auto returned = GetAll<S, Arg...>();
		for (auto& f : found)
		{
			for (auto& r : returned)
			{
				if (r == f)
				{
					total.emplace_back(f);
				}
			}
		}
		return total;
	}
}