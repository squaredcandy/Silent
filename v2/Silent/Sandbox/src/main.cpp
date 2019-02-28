#include <Silent.h>

class Sandbox : public Silent::Application
{
protected:
 
 	void SandboxLoopFunction()
 	{
		//static Uint32 lastTick = SDL_GetTicks();
		//Uint32 dt = SDL_GetTicks() - lastTick;
		//lastTick = SDL_GetTicks();
		//
 		////syss->RunSystems();
		//
		//static int i = 0;
		//static Uint32 timer = 0;
		//timer += dt;
	}

public:
	Sandbox() : Silent::Application()
	{
		platformType = SL_PLATFORM_SDL;
		rendererType = SL_RENDERER_GL3;


	}

	virtual ~Sandbox()
	{
	}
};

Silent::Application * Silent::CreateApplication()
{
	return new Sandbox();
}