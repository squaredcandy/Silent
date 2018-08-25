#pragma once

#include "Backend.h"
#include "BackendGL.h"

#include <SDL/SDL.h>
#include <ImGui/imgui.h>

#include <memory>

#define BACKEND_GL 0

namespace Silent::Engine
{
	extern SDL_Window * window;
	extern std::shared_ptr<Backend> backend;

	void InitEngine(int _backend);
	void GameLoop();
	void Cleanup();
}