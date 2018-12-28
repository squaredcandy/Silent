#pragma once


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

		Entity() = default;
		~Entity() = default;

		//Entity(Entity&&) = default;

		inline bool operator<(const Entity& other)
		{
			return _entityID < other._entityID;
		}

		inline bool operator==(const Entity& other)
		{
			return _entityID == other._entityID;
		}
	};
}