#pragma once
#include "../Core.h"
#include "System.h"
#include "System_Setting.h"
#include "System_Input.h"
#include "System_Camera.h"
#include "System_Light.h"
#include "System_Render.h"

#include "Systems.h"

namespace Silent
{
	// Class where we use imgui to display the debug stuff for all the 
	// various systems
	// We can extend this thing to a unity inspector type thing where we can 
	// look at entities and their modules but that will require that we do a 
	// func for displaying the various modules
	// All this stuff is gonna be super slow at run times cos the way we did
	// data structures, we'll see.
	class SILENT_API System_Debug : public System
	{
	private:
		Systems * _systems;
	public:
		System_Debug(Systems * systems = nullptr, 
					 SystemPriority priority = 10000) : 
			System("Debug", priority), _systems(systems) {}
		virtual ~System_Debug() = default;

		virtual void ForceUpdateModules(Modules& modules) override;
		virtual void IncrementalUpdateModules(Modules& modules) override;
		virtual void Execute() override;
		virtual void Cleanup() override;
	};
}