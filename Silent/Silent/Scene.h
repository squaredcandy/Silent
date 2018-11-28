// This file only exists because we need a place the stuff in our scene
#pragma once

#include "Entities.h"

namespace Silent::Engine::Scene
{
	extern EntityID playerID;
	extern std::vector<std::unique_ptr<Entity>> entities;
}