#pragma once

#include "../Core.h"

#include <memory>

namespace Silent
{
	// This class is a base template class so derived classes can inherit the 
	// singleton pattern
	template<typename T>
	class SILENT_API Singleton
	{
	public:
		static T& Instance();
		//static std::shared_ptr<T>& InstanceS(bool destroy = false);

	protected:
		//Singleton();
		//Singleton(Singleton const&);
		//Singleton& operator=(Singleton const&);
		//~Singleton();
	};
}