#pragma once
#include "../Core.h"
#include "LPlatform.h"
#include <SDL/SDL.h>

namespace Silent
{
	class SILENT_API LPlatformSDL : public LPlatform
	{
	protected:
	public:
		SDL_Window * window = nullptr;
		SDL_GLContext contextGL;

		LPlatformSDL(LRenderer * renderer = nullptr);
		virtual ~LPlatformSDL() = default;

		virtual void OnInitialize() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;
		virtual int GetWindowFlags(LRenderer * renderer) override;

		virtual char GetKeyChar(int key) override;
		virtual bool WindowResized(WindowResizedEvent& e) override;
		virtual bool WindowMoved(WindowMovedEvent& e) override;
	};
}

namespace Silent
{
	class SILENT_API SDL_GL_Compatibility
	{
	public:
		static void Init(SDL_Window * window, SDL_GLContext& context);
	};
}