#pragma once
#include "Core.h"
#include "Utility/Stacks.h"

#include "Resource/LResource.h"
#include "Entity/LEntity.h"
#include "Module/LModule.h"

#include <functional>

namespace Silent {

	class SILENT_API Application
	{
	protected:
		Stacks stack;

	public:
		int platformType = SL_PLATFORM_NONE;
		int rendererType = SL_RENDERER_NONE;

		Application();
		virtual ~Application();

		inline void Run();
	};

	// To be defined in the client
	Application * CreateApplication();
}