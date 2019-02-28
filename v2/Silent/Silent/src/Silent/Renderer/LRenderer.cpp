#include "LRenderer.h"
using namespace Silent;

void LRenderer::AddCallback()
{
	EventDispatcher<WindowResizedEvent>::AddCallback([&] (WindowResizedEvent& e)
	{
		size = { e.x, e.y };
		return false;
	});
}