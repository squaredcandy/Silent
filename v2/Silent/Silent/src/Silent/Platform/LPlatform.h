#pragma once
#include "../Core.h"
#include "../Layer/Layer.h"
#include "../Event/EventDispatcher.h"

namespace Silent
{
	class SILENT_API LRenderer;
	// This is another abstract layer which lets us abstract the platform stuff
	class SILENT_API LPlatform : public Layer
	{
	private:
		void DispatchCallback();
	protected:
		LRenderer * _renderer;
		std::string _name;
		int _width;
		int _height;
	public:
		const unsigned int _platformType;
		LPlatform(std::string name, int width, int height, 
				  unsigned int platformType, LRenderer * renderer) : Layer(), 
			_name(name), _width(width), _height(height), 
			_platformType(platformType), _renderer(renderer) 
		{
			DispatchCallback();
		}
		virtual ~LPlatform() = default;

		virtual int GetWindowFlags(LRenderer * renderer) = 0;
		virtual char GetKeyChar(int key) = 0;
		virtual bool WindowResized(WindowResizedEvent& e) = 0;
		virtual bool WindowMoved(WindowMovedEvent& e) = 0;
	};
}