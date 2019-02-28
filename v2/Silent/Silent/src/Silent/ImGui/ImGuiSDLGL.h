#pragma once
#include "../Core.h"
#include "LImGui.h"
#include "../Platform/LPlatformSDL.h"
#include "../Renderer/LRendererGL3.h"
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_opengl3.h>

// SDL / GL3 Implementation
namespace Silent
{
	class SILENT_API LPlatformSDL;
	class SILENT_API LRendererGL3;
	using LImGui_SDL_GL3 = LImGui<LPlatformSDL, LRendererGL3>;
}