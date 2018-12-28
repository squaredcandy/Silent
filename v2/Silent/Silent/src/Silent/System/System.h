#pragma once

#include "../Core.h"
#include "../Module/Modules.h"

namespace Silent
{
	using SystemPriority = int;
	class SILENT_API System
	{
	protected:
		bool _active = true;
		int _priority;
	public:
		System(SystemPriority priority = 0) : _priority(priority) {}
		virtual ~System() = default;
		virtual void UpdateEntities(Modules& modules) = 0;
		virtual void Execute() = 0;

		bool operator<(const System& other)
		{
			return _priority < other._priority;
		}
	};
}