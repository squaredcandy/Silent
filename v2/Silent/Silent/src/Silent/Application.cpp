#include "Application.h"
#include "Layer/Layers.h"
#include "Utility/Input.h"
#include "Utility/Debug.h"

#include "Interface/Interface.h"

namespace Silent
{
	Application::Application()
	{
		// The actual stuff
		auto * log = new LLog();
		Locator<LLog>::Provide(log);
		stack.PushMainLayer(log);
		Input::InitializeInput();

		// ECS stuff
		auto * entity = new LEntity();
		Locator<LEntity>::Provide(entity);
		stack.PushMainLayer(entity);

		auto * modules = new LModule();
		Locator<LModule>::Provide(modules);
		stack.PushMainLayer(modules);

		auto * newEnt = entity->AddEntity("Ent");
		auto * cam = modules->AddModule<MCamera>(newEnt);
		auto * light = modules->AddModule<MLight>(newEnt);
		auto * model = modules->AddModule<MModel>(newEnt);

		// should do this automaticly based on which enum user requests
		auto * rdr = new LRendererGL3();
		auto * plat = new LPlatformSDL(rdr);
		LImGui_SDL_GL3 * imgui = new LImGui(plat, rdr);

		// Here we upcast cast for better compatibility
		Locator<LPlatform>::Provide(plat);
		stack.PushMainLayer(plat);

		Locator<LRenderer>::Provide(rdr);
		stack.PushMainLayer(rdr);

		Locator<LImGui_SDL_GL3>::Provide(imgui);
		stack.PushMainLayer(imgui);

		// Resources
// 		auto * resources = new LResource();
// 		Locator<LResource>::Provide(resources);
// 		stack.PushMainLayer(resources);

		// Debug
		auto * debug = new LDebug();
		Locator<LDebug>::Provide(debug);
		stack.PushMainLayer(debug);
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		bool run = true;
		auto quit = [&] (QuitEvent& event) 
		{
			run = false;
			return true;
		};
		EventDispatcher<QuitEvent>::AddCallback(quit);
		auto * imgui = Locator<LImGui_SDL_GL3>::Get();
		while (run)
		{
			stack.UpdateStacks();
			imgui->Render();
		}
	}
}