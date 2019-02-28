#pragma once

#include "../Core.h"
#include "Event.h"

namespace Silent
{
	class SILENT_API Entity;
	class SILENT_API Module;
	class SILENT_API ModuleAddedEvent : public Event
	{
	public:
		Entity * entity;
		Module * module;
		virtual ~ModuleAddedEvent() = default;
	};
	class SILENT_API ModuleRemovedEvent : public Event
	{
	public:
		Entity * entity;
		Module * module;
		virtual ~ModuleRemovedEvent() = default;
	};
}