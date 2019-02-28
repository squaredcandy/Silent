#include "LPlatform.h"
using namespace Silent;

void LPlatform::DispatchCallback()
{
	WindowResizedEvent e;
	e.x = _width;
	e.y = _height;
	EventDispatcher<WindowResizedEvent>::Dispatch(&e);
}
