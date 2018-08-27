#pragma once

#include "Resources.h"
#include "Backend.h"
#include "BackendSDLGL.h"
#include "Systems.h"

#include <SDL/SDL.h>
#include <ImGui/imgui.h>

#include <memory>

#define BACKEND_SDL_GL
#define BACKEND_DEFAULT


namespace Silent::Engine
{
	extern std::shared_ptr<Backend> backend;

	void InitEngine();
	void GameLoop();
	void Cleanup();
}