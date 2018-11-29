#include "Engine.h"

#include "Log.h"

namespace Silent::Engine
{
	std::shared_ptr<Backend> backend;
	std::function<void()> loopFunction;
	std::function<void()> windowFunction;

	void InitEngine()
	{
#if defined BACKEND_SDL_GL
		backend = std::make_shared<BackendSDLGL>();
#elif defined BACKEND_DEFAULT
		backend = std::make_shared<BackendSDLGL>();
#endif // BACKEND

		backend->Init();
		backend->SettingsInit();
	}

	void GameLoop()
	{
		bool done = false;
		while (!done)
		{
			done = backend->UpdateEvent();
			backend->Update();

			Systems::Update();
			Systems::Execute();

			loopFunction();
			windowFunction();

 			backend->Render();
		}
	}

	void Cleanup()
	{
		Resources::CleanupResources();
		backend->Cleanup();
	}
}