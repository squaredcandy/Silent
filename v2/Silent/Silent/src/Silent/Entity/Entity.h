#pragma once
#include "../Core.h"

#include <bitset>
#define MAX_COMPONENTS 64
namespace Silent
{
	using EntityID = unsigned int;

	inline EntityID GenerateEntityID()
	{
		// ID = 0 would be invalid
		static EntityID id = 1;
		return id++;
	}

	class SILENT_API Module;

	class SILENT_API Entity
	{
	public:
		const EntityID _entityID = GenerateEntityID();
		std::bitset<MAX_COMPONENTS> components;
		bool _active = true;

		Entity() = default;
		~Entity() = default;

		inline bool operator<(const Entity& other) const
		{
			return _entityID < other._entityID;
		}
		inline bool operator==(const Entity& other) const
		{
			return _entityID == other._entityID;
		}
		inline void AddComponent(unsigned int id)
		{
			components[id] = true;
		}
		inline void RemoveComponent(unsigned int id)
		{
			components[id] = false;
		}
		inline bool HasComponent(unsigned int id)
		{
			return components[id];
		}
	};
}