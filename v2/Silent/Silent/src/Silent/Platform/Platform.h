#pragma once

#include "../Core.h"
#include "../Renderer/Renderers.h"

namespace Silent
{
	class SILENT_API Platform
	{
	protected:
		virtual int PreClear() = 0;
		virtual int PostClear() = 0;
		virtual int Flip() = 0;
	public:
		// The renderer we are using to draw stuff
		Renderer * renderer;

		virtual ~Platform() = default;

		virtual inline int Initialize() = 0;
		virtual inline int UpdateIO() = 0;
		virtual inline int NewFrame() = 0;
		virtual inline int Update() = 0;
		virtual inline int Render() = 0;
		virtual inline int Clear() = 0;
		virtual inline int Cleanup() = 0;
	};
	// To be defined in platforms.cpp
	Platform * GetPlatform(int platformType);
}