#include "Application.h"

namespace Silent
{
	Application::Application()
	{
	}

	Application::~Application()
	{
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