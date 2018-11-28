#pragma once

#include "System.h"

class TestSystem : public System
{
public:
	TestSystem() = default;
	virtual ~TestSystem() = default;

	void UpdateEntities() override
	{
		targets = Silent::Engine::Entities::GetAll<ModTransform>(Silent::Engine::Scene::entities, true);
	}

	void Execute() override
	{
		float dt = ImGui::GetIO().DeltaTime;
		for(auto t : targets)
		{
			t->GetTransform().GetTranslate().y += 1.f * dt;
		}
	}
};