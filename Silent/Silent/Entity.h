#pragma once

#include "Module.h"
#include <stdexcept>

using EntityID = std::size_t;

inline EntityID GenerateNewEntityID()
{
	static EntityID lastId = 0;
	return lastId++;
}

class ModCommon;
class ModTransform;

class Entity
{
private:
	EntityID id = GenerateNewEntityID();
	bool active = true;
	std::vector<std::unique_ptr<Module>> modules;
	std::vector<std::unique_ptr<Entity>> childEntities;

	ModuleBitSet moduleBitset;
	ModuleArray moduleArray;

public:
	virtual ~Entity() = default;

	auto& GetChildren() { return childEntities; }

	void Update(float deltaTime)
	{
		for (auto& m : modules) m->Update(deltaTime);
	}

	void GUI()
	{
		ImGui::PushID((int)id);
		for (auto& m : modules) m->GUI();
		ImGui::PopID();
	}

	bool isActive() const { return active; }
	void destroy() { active = false; }

	// See if it has module
	template <typename T> bool HasMod() const
	{
		return moduleBitset[GetModTypeID<T>()];
	}

	template <typename T, typename... TArgs> T& AddMod(TArgs&&... mArgs)
	{
		// Don't allow duplicate Modules
		if (!HasMod<T>())
		{
			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->entity = this;
			std::unique_ptr<Module> uPtr{ c };
			modules.emplace_back(std::move(uPtr));

			moduleArray[GetModTypeID<T>()] = c;
			moduleBitset[GetModTypeID<T>()] = true;

			c->Init();
			return *c;
		}
		else
		{
			return GetMod<T>();
		}
	}

	inline bool operator==(const Entity& e) { return (this->id == e.id) ? true : false; }
	inline bool operator!=(const Entity& e) { return !(*this == e); }

	EntityID GetID() const { return id; }

	template<typename T> T& GetMod() const
	{
		return *dynamic_cast<T*>(moduleArray[GetModTypeID<T>()]);
	}

	ModCommon& GetCommon()
	{
		return *(ModCommon*) (moduleArray[GetModTypeID<ModCommon>()]);
	}
	ModTransform& GetTransform()
	{
		return *(ModTransform*) (moduleArray[GetModTypeID<ModTransform>()]);
	}
};