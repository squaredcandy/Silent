#pragma once
#include "../Core.h"
#include "System.h"

namespace Silent
{
	class SILENT_API SInput : public System
	{
	protected:
		// Something to do with the input stuff 
	public:
		SInput(SystemPriority priority = 0) : System("Input", priority){}
		virtual ~SInput() = default;

		virtual void ForceUpdateModules(Modules& modules) override;
		virtual void IncrementalUpdateModules(Modules& modules) override;
		virtual void Execute() override;
		virtual void Cleanup() override;
	};
}