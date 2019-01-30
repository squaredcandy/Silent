#pragma once
#include "../Core.h"
#include "System.h"

namespace Silent
{
	// Store settings for other systems?
	// The real question is if we want to use a map of string to anything
	// that way we can sort of do an auto imgui thing but that will mean 
	// its runtime only or we can just do hard values
	class SILENT_API System_Setting : public System
	{
	public:
		System_Setting(SystemPriority priority = 20) 
			: System("Settings", priority) {}
		virtual ~System_Setting() = default;

		virtual void ForceUpdateModules(Modules& modules) override;
		virtual void IncrementalUpdateModules(Modules& modules) override;
		virtual void Execute() override;
		virtual void Cleanup() override;
	};

}