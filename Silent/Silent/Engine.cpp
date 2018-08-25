#include "Engine.h"

namespace Silent::Engine
{
	SDL_Window * window;
	std::shared_ptr<Backend> backend;

	void InitEngine(int _backend)
	{
		if (_backend == BACKEND_GL) backend = std::make_shared<BackendGL>();

		// Init SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("SDL Error: %s\n", SDL_GetError());
		}

		backend->Init(window);
		backend->SettingsInit();
	}

	void GameLoop()
	{
		bool done = false;
		while (!done)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				ImGui_ImplSDL2_ProcessEvent(&event);
				if (event.type == SDL_QUIT)
					done = true;
			}
			backend->Update();
			ImGui_ImplSDL2_NewFrame(window);
			ImGui::NewFrame();

 			backend->Render(window);
		}
	}

	void Cleanup()
	{
		backend->Cleanup();

		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}