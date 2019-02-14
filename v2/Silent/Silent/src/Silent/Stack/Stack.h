#pragma once

#include "../Core.h"

namespace Silent
{
	// So the idea of this class provide a framework where we can just 
	// add derivatives of this class to a vector 
	class Stack
	{
	public:
		bool active = true;
		Stack() = default;
		virtual ~Stack() = default;

		virtual void OnInitialize() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnDestroy() = 0;
	};
}