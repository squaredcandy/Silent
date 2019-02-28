#include "LPlatformSDL.h"
#include "../Renderer/LRenderer.h"

using namespace Silent;

LPlatformSDL::LPlatformSDL(LRenderer * renderer) :
	LPlatform("Silent Engine", 640, 360, SL_PLATFORM_SDL, renderer)
{
}

bool LPlatformSDL::WindowResized(WindowResizedEvent& e)
{
	_width = e.x;
	_height = e.y;
	SDL_SetWindowSize(window, _width, _height);
	return false;
}

bool LPlatformSDL::WindowMoved(WindowMovedEvent& e)
{
	SDL_SetWindowPosition(window, e.x, e.y);
	return false;
}


void LPlatformSDL::OnInitialize()
{
	// Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		printf("SDL Error: %s\n", SDL_GetError());
	}

	//SDL_DisplayMode current;
	//SDL_GetCurrentDisplayMode(0, &current);

	int windowFlags = GetWindowFlags(_renderer);

	window = SDL_CreateWindow(_name.c_str(), 
							  SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED, 
							  _width, _height, windowFlags);

	//Here we switch between the different renderers eg Opengl/Vulkan
	if (_renderer->_rendererType == SL_RENDERER_GL3)
	{
		SDL_GL_Compatibility::Init(window, contextGL);
	}
	EventDispatcher<WindowResizedEvent>::
		AddCallback(BIND_1(LPlatformSDL::WindowResized));
	EventDispatcher<WindowMovedEvent>::
		AddCallback(BIND_1(LPlatformSDL::WindowMoved));
}

void LPlatformSDL::OnUpdate()
{
}

void LPlatformSDL::OnDestroy()
{
	if (_renderer->_rendererType == SL_RENDERER_GL3)
	{
		SDL_GL_DeleteContext(contextGL);
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

}

int LPlatformSDL::GetWindowFlags(LRenderer * renderer)
{
	if (renderer->_rendererType == SL_RENDERER_GL3)
	{
		return SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | 
			SDL_WINDOW_ALLOW_HIGHDPI;
	}
	return 0;
}

char LPlatformSDL::GetKeyChar(int key)
{
	return SDL_GetKeyFromScancode((SDL_Scancode) key);
}

#define RGB_BUFFER_SIZE 8
#define DEPTH_BUFFER_SIZE 24
#define STENCIL_BUFFER_SIZE 8
#define OPENGL_MAJOR_VER 4
#define OPENGL_MINOR_VER 3

void SDL_GL_Compatibility::Init(SDL_Window * window, SDL_GLContext& context)
{
	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG
						| SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
						SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, RGB_BUFFER_SIZE);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, RGB_BUFFER_SIZE);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, RGB_BUFFER_SIZE);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, RGB_BUFFER_SIZE);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_BUFFER_SIZE);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, STENCIL_BUFFER_SIZE);

	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, OPENGL_MAJOR_VER);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, OPENGL_MINOR_VER);

	context = SDL_GL_CreateContext(window);
	SDL_GL_SetSwapInterval(0); // Enable vsync
}
