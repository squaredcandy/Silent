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
		MapTypeToConModulePtr _modules;
	public:
		const std::string _name;

		System(std::string name, SystemPriority priority = 0) 
			: _name(name), _priority(priority) {}
		virtual ~System() = default;
		
		bool operator<(const System& other) const
		{
			return _priority < other._priority;
		}

		virtual inline void RemoveNullModules()
		{
			for (auto&[key, val] : _modules)
			{
				for (auto cont = val.begin(); cont != val.end();)
				{
					if ((*cont) == nullptr) cont = val.erase(cont);
					else ++cont;
				}
			}
		}

		// This basiclly tells the system to start from scratch
		virtual void ForceUpdateModules(Modules& modules) = 0;

		// This is a faster version where we just add and remove the relevent
		// modules
		virtual void IncrementalUpdateModules(Modules& modules) = 0;

		// Display Debug Info
		virtual void DebugInfo() {}

		virtual void Execute() = 0;
		virtual void Cleanup() = 0;
	};
}