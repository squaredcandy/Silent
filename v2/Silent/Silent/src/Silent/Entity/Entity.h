#pragma once

#include <set>
#include <typeindex>

namespace Silent
{
	using EntityID = unsigned int;

	inline EntityID GenerateEntityID()
	{
		// ID = 0 would be invalid
		static EntityID id = 1;
		return id++;
	}

	class SILENT_API Entity
	{
	public:
		EntityID _entityID = GenerateEntityID();
		bool _active = true;

		std::set<std::type_index> modules;

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
	};
}