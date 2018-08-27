#pragma once

#include "Module.h"

using EntityID = std::size_t;

class ModCommon : public Module
{
public:
	std::string name;
	ModCommon(std::string _name = "") : 
		name(_name)
	{

	}

	virtual ~ModCommon() override = default;

	void Init() override
	{

	}

	void Update(float deltaTime) override
	{

	}

	void GUI() override
	{
		if (ImGui::TreeNode("Common Module"))
		{
			ImGui::TreePop();
		}
	}
};