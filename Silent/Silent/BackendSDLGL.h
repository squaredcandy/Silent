#pragma once

#include "Backend.h"

#include <SDL/SDL.h>
#include <GL/gl3w.h>
#include <ImGui/imgui.h>
#include <ImGui/imgui_impl_sdl.h>
#include <ImGui/imgui_impl_opengl3.h>

#include <memory>

#define RGB_BUFFER_SIZE 8
#define DEPTH_BUFFER_SIZE 24
#define STENCIL_BUFFER_SIZE 8

#define OPENGL_MAJOR_VER 4
#define OPENGL_MINOR_VER 3

class BackendSDLGL : public Backend
{
public:
	SDL_Window * window;
	SDL_GLContext context;

	BackendSDLGL();
	virtual ~BackendSDLGL();

	void Init() override;
	void SettingsInit()	override;
	bool UpdateEvent() override;
	void Update() override;
	void Clear() override;
	void Render() override;
	void Cleanup() override;

	void SetVSync(bool on) override;
	void ResizeWindow(int w, int h) override;
};

