#pragma once

#include "../Core.h"

namespace Silent
{
	class SILENT_API Event
	{
	public:
		bool handled = false;
		virtual ~Event() = default;

	};
	class SILENT_API KeyEvent : public Event
	{
	public:
		int pressed = -1;
		int key = -1;
		int repeat = -1;
		virtual ~KeyEvent() = default;
	};
	class SILENT_API MouseEvent : public Event
	{
	public:
		int button = -1;
		int pressed = -1;
		int clicks = -1;
		int x = -1, y = -1;
		virtual ~MouseEvent() = default;
	};
	class SILENT_API MouseMotionEvent : public Event
	{
	public:
		int x = -1, y = -1;
		int xRel = -1, yRel = -1;
		virtual ~MouseMotionEvent() = default;
	};
	class SILENT_API MouseWheelEvent : public Event
	{
	public:
		int x = -1, y = -1;
		virtual ~MouseWheelEvent() = default;
	};
	class SILENT_API WindowMovedEvent : public Event
	{
	public:
		int x = -1, y = -1;
		virtual ~WindowMovedEvent() = default;
	};
	class SILENT_API WindowResizedEvent : public Event
	{
	public:
		int x = -1, y = -1;
		virtual ~WindowResizedEvent() = default;
	};
	class SILENT_API QuitEvent : public Event
	{
	public:
		virtual ~QuitEvent() = default;
	};
}