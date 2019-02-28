#pragma once
#include "../Core.h"
#include "Entity.h"
#include "../Layer/Layer.h"
#include <glm/vec3.hpp>

#include <bitset>

namespace Silent
{
	class SILENT_API LEntity : public Layer
	{
	private:
		std::vector<Entity*> entities;

	public:
		LEntity() = default;
		~LEntity() = default;

		Entity * AddEntity(std::string name = "", Entity * parent = nullptr,
						   glm::vec3 translation = glm::vec3(),
						   glm::vec3 rotation = glm::vec3(),
						   glm::vec3 scale = glm::vec3(1));
		void RemoveEntity(EntityID id);
		void Cleanup();

		std::vector<EntityID> EntitiesThatMatch(std::bitset<MAX_COMPONENTS> key);

		inline void OnInitialize() override;
		inline void OnUpdate() override;
		inline void OnDestroy() override;
	};
}