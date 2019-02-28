#pragma once

#include "../Core.h"

namespace Silent
{
	// So the idea of this class provide a framework where we can just 
	// add derivatives of this class to a vector 
	class SILENT_API Layer
	{
	public:
		bool active = true;
		Layer() = default;
		virtual ~Layer() = default;

		virtual void OnInitialize() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDestroy() = 0;
	};
}