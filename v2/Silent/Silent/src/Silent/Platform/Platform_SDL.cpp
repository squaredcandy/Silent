#include "Platform_SDL.h"

#include <stdio.h>
#include <ImGui/imgui_impl_sdl.h>

namespace Silent
{
	int Platform_SDL::Initialize()
	{
		// Init SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			printf("SDL Error: %s\n", SDL_GetError());
			return SL_ERROR_INIT;
		}

		renderer->PreWindowInitialization();

		SDL_DisplayMode current;
		SDL_GetCurrentDisplayMode(0, &current);

		auto windowFlags = renderer->GetWindowFlags();

		window = SDL_CreateWindow("Silent Engine",
								  SDL_WINDOWPOS_CENTERED,
								  SDL_WINDOWPOS_CENTERED,
								  1600, 900, windowFlags);

		renderer->PostWindowInitialization(window);

		// Setup ImGui binding
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		renderer->ImGuiInitialization(window);
		renderer->SettingsInitialization();

		return SL_ERROR_NONE;
	}

	int Platform_SDL::UpdateIO()
	{
		int notify = SL_NOTIFY_NONE;
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				notify = SL_NOTIFY_QUIT;
		}
		return notify;
	}

	int Platform_SDL::NewFrame()
	{
		renderer->NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();
		return SL_ERROR_NONE;
	}

	int Platform_SDL::Update()
	{
		return SL_ERROR_NONE;
	}

	int Platform_SDL::Render()
	{
		PreClear();
		Clear();
		PostClear();
		Flip();
		return SL_ERROR_NONE;
	}

	int Platform_SDL::PreClear()
	{
		ImGui::Render();
		renderer->PreClear(window);
		return SL_ERROR_NONE;
	}
	
	int Platform_SDL::Clear()
	{
		renderer->Clear();
		return SL_ERROR_NONE;
	}

	int Platform_SDL::PostClear()
	{
		renderer->PostClear();
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
		return SL_ERROR_NONE;
	}

	int Platform_SDL::Flip()
	{
		renderer->Flip(window);
		return SL_ERROR_NONE;
	}

	int Platform_SDL::Cleanup()
	{
		renderer->Cleanup(window);
		delete renderer;
		return SL_ERROR_NONE;
	}

}