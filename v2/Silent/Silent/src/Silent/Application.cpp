#include "Application.h"
#include "Utility/Singleton.h"

namespace Silent
{
	Application::Application()
	{
		ress = &Singleton<Resources>::Instance();
		ents = &Singleton<Entities>::Instance();
		mods = &Singleton<Modules>::Instance();
		syss = &Singleton<Systems>::Instance();

		settings = syss->AddSystem<System_Setting>();
		debug = syss->AddSystem<System_Debug>(syss);
	}

	Application::~Application()
	{
		delete ress;
		delete ents;
		delete mods;
		delete syss;
	}

	void Application::Run()
	{
		platform = GetPlatform(platformType);
		platform->renderer = GetRenderer(rendererType);
		
		Init();
		Gameloop();
		Cleanup();
	}

	void Application::Init()
	{
		platform->Initialize();
		initFunction();
	}

	void Application::Gameloop()
	{
		while (true)
		{
			if(platform->UpdateIO() == SL_NOTIFY_QUIT) break;
			platform->NewFrame();
			loopFunction();
			platform->Update();
			platform->Render();
		}
	}

	void Application::Cleanup()
	{
		platform->Cleanup();
		delete platform;
	}
}