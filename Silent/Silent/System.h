#pragma once

#include "Mods.h"
#include "Entities.h"

class System
{
private:
public:
	bool enabled = true;
	std::vector<Entity*> targets;

	virtual ~System() = default;

	virtual void UpdateEntities() = 0;
	virtual void Execute() = 0;
};