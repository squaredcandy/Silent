#pragma once

#include "Platform.h"

#include <SDL/SDL.h>

namespace Silent
{
	class SILENT_API Platform_SDL : public Platform
	{
	protected:
		SDL_Window * window;

		int inline PreClear() override;
		int inline PostClear() override;
		int inline Flip() override;

	public:
		int inline Initialize() override;
		int inline UpdateIO() override;
		int inline NewFrame() override;
		int inline Update() override;
		int inline Render() override;
		int inline Clear() override;
		int inline Cleanup() override;
	};
}