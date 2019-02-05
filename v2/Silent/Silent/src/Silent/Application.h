#pragma once
#include "Core.h"
#include "Platform/Platforms.h"

#include <functional>

#include "Resource/Resources.h"
#include "Entity/Entities.h"
#include "Module/Modules.h"
#include "System/Systems.h"

#include "System/SDebug.h"

namespace Silent {

	class SILENT_API Application
	{
	protected:
		Platform * platform;

		Resources * ress;
		Entities * ents;
		Modules * mods;
		Systems * syss;

		SSetting * settings;
		SDebug * debug;

	public:
		int platformType = SL_PLATFORM_NONE;
		int rendererType = SL_RENDERER_NONE;
		std::function<void()> initFunction;
		std::function<void()> loopFunction;

		Application();
		virtual ~Application();

		inline void Run();
		inline void Init();
		inline void Gameloop();
		inline void Cleanup();
	};

	// To be defined in the client
	Application * CreateApplication();
}