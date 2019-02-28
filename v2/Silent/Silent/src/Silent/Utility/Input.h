#pragma once
#include "../Core.h"
#include "../Event/EventDispatcher.h"

namespace Silent
{
	class SILENT_API Input
	{
	protected:
		inline static KeyEvent ke;
		inline static MouseEvent me;
		inline static MouseMotionEvent mme;
		inline static MouseWheelEvent mwe;
		inline static WindowMovedEvent wme;
		inline static WindowResizedEvent wre;
		inline static QuitEvent qe;
	public:
		static void InitializeInput();

		static bool LeftMouseDown();
		static bool MiddleMouseDown();
		static bool RightMouseDown();
		static int MouseDownPosX();
		static int MouseDownPosY();

		static int MouseX();
		static int MouseY();
		static int MouseXRel();
		static int MouseYRel();

		static int MouseWheelX();
		static int MouseWheelY();

		static int WindowPosX();
		static int WindowPosY();

		static int WindowX();
		static int WindowY();

		static bool KeyDown(int key);
		static bool Key(int key);
		static bool KeyUp(int key);
	};
}