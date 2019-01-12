#include "Entities.h"
#include "Singleton.h"
#include "../Module/Modules.h"

namespace Silent
{

	const std::vector<std::shared_ptr<Silent::Entity>>& Entities::GetEntities()
	{
		return entities;
	}

	std::shared_ptr<Entity> Entities::AddEntity(
		std::string name,
		std::shared_ptr<Entity> parent,
		glm::vec3 translation,
		glm::vec3 rotation,
		glm::vec3 scale)
	{
		std::shared_ptr<Entity> e = std::make_shared<Entity>();
		Singleton<Modules>::Instance().AddModule<Module_Transform>(e, 
			name, parent, translation, rotation, scale);
		entities.emplace_back(e);
		return entities.back();
	}
}