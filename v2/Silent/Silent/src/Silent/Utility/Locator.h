#pragma once
#include "../Core.h"

namespace Silent
{
	// Service Locator Pattern
	template<class T>
	class SILENT_API Locator
	{
	private:
		inline static T * _instance = nullptr;
	public:
		static void Provide(T * instance) { _instance = instance; }
		static T * Get()
		{
			assert(_instance != nullptr);
			return _instance;
		}
	};
}