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
		
		// This basiclly tells the system to start from scratch
		virtual void ForceUpdateModules(Modules& modules) = 0;

		// This is a faster version where we just add and remove the relevent
		// modules
		virtual void IncrementalUpdateModules(Modules& modules) = 0;

		virtual void Execute() = 0;
		virtual void Cleanup() = 0;
		bool operator<(const System& other) const
		{
			return _priority < other._priority;
		}
	};
}