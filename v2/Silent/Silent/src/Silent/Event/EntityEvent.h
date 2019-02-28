#pragma once

#include "../Core.h"
#include "Event.h"

namespace Silent
{
	class SILENT_API Entity;
	class SILENT_API EntityAddedEvent : public Event
	{
	public:
		Entity * entity;
		std::string name;
		Entity * parent;
		glm::vec3 transform[3];
		virtual ~EntityAddedEvent() = default;
	};
	class SILENT_API EntityRemovedEvent : public Event
	{
	public:
		Entity * entity;
		virtual ~EntityRemovedEvent() = default;
	};
}