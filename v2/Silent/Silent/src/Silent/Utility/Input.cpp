#include "Input.h"

using namespace Silent;

void Input::InitializeInput()
{
	// Init values
	mme.x = 0;
	mme.y = 0;
	mme.xRel = 0;
	mme.yRel = 0;

	mwe.x = 0;
	mwe.y = 0;

	//auto pos = ImGui::GetWindowPos();
	//wme.x = (int) pos.x;
	//wme.y = (int) pos.y;
	//
	//auto size = ImGui::GetWindowSize();
	//wre.x = (int) size.x;
	//wre.y = (int) size.y;

	const auto newKeyEvent = [&] (KeyEvent& e)
	{
		ke = e;
		return false;
	};
	const auto newMouseEvent = [&] (MouseEvent& e)
	{
		me = e;
		return false;
	};
	const auto newMouseMotionEvent = [&] (MouseMotionEvent& e)
	{
		mme = e;
		return false;
	};
	const auto newMouseWheelEvent = [&] (MouseWheelEvent& e)
	{
		mwe = e;
		return false;
	};
	const auto newWindowMovedEvent = [&] (WindowMovedEvent& e)
	{
		wme = e;
		return false;
	};
	const auto newWindowResizedEvent = [&] (WindowResizedEvent& e)
	{
		wre = e;
		return false;
	};
	const auto newQuitEvent = [&] (QuitEvent& e)
	{
		qe = e;
		return false;
	};

	EventDispatcher<KeyEvent>::AddCallback(newKeyEvent);
	EventDispatcher<MouseEvent>::AddCallback(newMouseEvent);
	EventDispatcher<MouseMotionEvent>::AddCallback(newMouseMotionEvent);
	EventDispatcher<MouseWheelEvent>::AddCallback(newMouseWheelEvent);
	EventDispatcher<WindowMovedEvent>::AddCallback(newWindowMovedEvent);
	EventDispatcher<WindowResizedEvent>::AddCallback(newWindowResizedEvent);
	EventDispatcher<QuitEvent>::AddCallback(newQuitEvent);
}

bool Input::LeftMouseDown()
{
	return (me.pressed && me.button == 1);
}
bool Input::MiddleMouseDown()
{
	return (me.pressed && me.button == 2);
}
bool Input::RightMouseDown()
{
	return (me.pressed && me.button == 3);
}
int Input::MouseDownPosX()
{
	return me.x;
}
int Input::MouseDownPosY()
{
	return me.y;
}
int Input::MouseX()
{
	return mme.x;
}
int Input::MouseY()
{
	return mme.y;
}
int Input::MouseXRel()
{
	return mme.xRel;
}
int Input::MouseYRel()
{
	return mme.yRel;
}
int Input::MouseWheelX()
{
	return mwe.x;
}
int Input::MouseWheelY()
{
	return mwe.y;
}
int Input::WindowPosX()
{
	return wme.x;
}
int Input::WindowPosY()
{
	return wme.y;
}
int Input::WindowX()
{
	return wre.x;
}
int Input::WindowY()
{
	return wre.y;
}
bool Input::KeyDown(int key)
{
	return (ke.key == key && ke.pressed && !ke.repeat);
}
bool Input::Key(int key)
{
	return (ke.key == key && ke.pressed);
}
bool Input::KeyUp(int key)
{
	return (ke.key == key && !ke.pressed);
}