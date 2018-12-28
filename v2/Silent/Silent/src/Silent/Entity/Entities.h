#pragma once
#include "../Core.h"
#include "Entity.h"

#include <glm/vec3.hpp>

namespace Silent
{
	// This class is meant to be used with a singleton
	class SILENT_API Entities
	{
	private:
		std::vector<std::shared_ptr<Entity>> entities;

	public:
		std::shared_ptr<Entity> AddEntity(std::string name = "",
										  std::shared_ptr<Entity> parent = nullptr,
										  glm::vec3 translation = glm::vec3(),
										  glm::vec3 rotation = glm::vec3(),
										  glm::vec3 scale = glm::vec3(1));
	};
}