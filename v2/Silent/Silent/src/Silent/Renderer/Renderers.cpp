#include "Renderers.h"

namespace Silent
{
	Renderer * GetRenderer(int rendererType)
	{
		if (rendererType == SL_RENDERER_GL3) return new Renderer_GL3();
		return nullptr;
	}
}