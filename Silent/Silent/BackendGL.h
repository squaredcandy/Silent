#pragma once

#include "Backend.h"

#include <SDL/SDL.h>
#include <GL/gl3w.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_opengl3.h>

#define RGB_BUFFER_SIZE 8
#define DEPTH_BUFFER_SIZE 24
#define STENCIL_BUFFER_SIZE 8

#define OPENGL_MAJOR_VER 4
#define OPENGL_MINOR_VER 3

class BackendGL : public Backend
{
public:
	SDL_GLContext context;

	BackendGL();
	virtual ~BackendGL();

	void Init(SDL_Window *& window) override;
	void SettingsInit()	override;
	void Update() override;
	void Clear() override;
	void Render(SDL_Window * window) override;
	void Cleanup() override;
};

