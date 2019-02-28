#include "ImGuiSDLGL.h"
#include "../Event/EventDispatcher.h"
#include "../Layer/LLog.h"
#include "../Utility/Stacks.h"
using namespace Silent;

template<>
void LImGui<LPlatformSDL, LRendererGL3>::OnInitialize()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplSDL2_InitForOpenGL(_platform->window, _platform->contextGL);
	ImGui_ImplOpenGL3_Init(/*"#version 410 core"*/);
	ChangeColors();
}
void HandleWindowEvents(SDL_Event& event)
{
// 	if (event.window.event == SDL_WINDOWEVENT_SHOWN)
// 	{
// 
// 	}
// 	else if (event.window.event == SDL_WINDOWEVENT_HIDDEN)
// 	{
// 
// 	}
// 	else if (event.window.event == SDL_WINDOWEVENT_EXPOSED)
// 	{
// 		// render only if this is exposed?
// 	}
	if (event.window.event == SDL_WINDOWEVENT_MOVED)
	{
		WindowMovedEvent wme;
		wme.x = event.window.data1;
		wme.y = event.window.data2;
		EventDispatcher<WindowMovedEvent>::Dispatch(&wme);
		LOG("Window Moved: (%d, %d)", wme.x, wme.y);
		
	}
	else if (event.window.event == SDL_WINDOWEVENT_RESIZED)
	{
		WindowResizedEvent wre;
		wre.x = event.window.data1;
		wre.y = event.window.data2;
		EventDispatcher<WindowResizedEvent>::Dispatch(&wre);
		LOG("Window Resized: (%d, %d)", wre.x, wre.y);
	}
// 	else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
// 	{
// 
// 	}
// 	else if (event.window.event == SDL_WINDOWEVENT_MINIMIZED)
// 	{
// 
// 	}
// 	else if (event.window.event == SDL_WINDOWEVENT_MAXIMIZED)
// 	{
// 
// 	}
// 	else if (event.window.event == SDL_WINDOWEVENT_RESTORED)
// 	{
// 
// 	}
// 	else if (event.window.event == SDL_WINDOWEVENT_ENTER)
// 	{
// 
// 	}
// 	else if (event.window.event == SDL_WINDOWEVENT_LEAVE)
// 	{
// 
// 	}
// 	else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
// 	{
// 
// 	}
// 	else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
// 	{
// 
// 	}
// 	else if (event.window.event == SDL_WINDOWEVENT_CLOSE)
// 	{
// 
// 	}
}
template<>
void LImGui<LPlatformSDL, LRendererGL3>::OnUpdate()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP)
		{
			KeyEvent ke;
			ke.pressed = event.key.state;
			ke.key = event.key.keysym.scancode;
			ke.repeat = event.key.repeat;
			EventDispatcher<KeyEvent>::Dispatch(&ke);
			LOG("Keypress: %s %c key, repeated %d times",
				((ke.pressed) ? "Pressed" : "Released"),
 				_platform->GetKeyChar(ke.key), ke.repeat);
		}
		else if (event.type == SDL_MOUSEBUTTONDOWN || 
				 event.type == SDL_MOUSEBUTTONUP)
		{
			MouseEvent me;
			me.button = event.button.button;
			me.pressed = event.button.state;
			me.clicks = event.button.clicks;
			me.x = event.button.x;
			me.y = event.button.y;
			EventDispatcher<MouseEvent>::Dispatch(&me);
			LOG("Mouse Button: %d %s, repeated %d times", 
						me.button, (me.pressed ? "Down" : "Up"), me.clicks);
		}
		else if (event.type == SDL_MOUSEMOTION)
		{
			MouseMotionEvent mme;
			mme.x = event.motion.x;
			mme.y = event.motion.y;
			mme.xRel = event.motion.xrel;
			mme.yRel = event.motion.yrel;
			EventDispatcher<MouseMotionEvent>::Dispatch(&mme);
			LOG("Mouse Motion: (%d, %d) Rel (%d, %d)",
						mme.x, mme.y, mme.xRel, mme.yRel);
		}
		else if (event.type == SDL_MOUSEWHEEL)
		{
			MouseWheelEvent mw;
			mw.x = event.wheel.x;
			mw.y = event.wheel.y;
			EventDispatcher<MouseWheelEvent>::Dispatch(&mw);
			LOG("Mouse Wheel: (%d, %d)", mw.x, mw.y);
		}
		else if (event.type == SDL_WINDOWEVENT)
		{
			HandleWindowEvents(event);
		}
		else if (event.type == SDL_QUIT)
		{
			QuitEvent qe;
			EventDispatcher<QuitEvent>::Dispatch(&qe);
		}
	}
	NewFrame();
 	bool a = true;
 	ImGui::ShowDemoWindow(&a);
}
template<>
void LImGui<LPlatformSDL, LRendererGL3>::OnDestroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}
template<>
void LImGui<LPlatformSDL, LRendererGL3>::NewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(_platform->window);
	ImGui::NewFrame();
}
template<>
void LImGui<LPlatformSDL, LRendererGL3>::Render()
{
	// Pre Clear
	ImGui::Render();
	SDL_GL_MakeCurrent(_platform->window, _platform->contextGL);

	// Clear
	auto& io = ImGui::GetIO();
	_renderer->Clear();

	// Post Clear
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	// Flip
	SDL_GL_SwapWindow(_platform->window);
}