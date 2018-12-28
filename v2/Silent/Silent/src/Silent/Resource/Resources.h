#pragma once

#include <map>
#include <memory>
#include <string>
#include <iostream>
#include <filesystem>

#include "../Core.h"
#include "Resource.h"

namespace Silent
{
	namespace fs = std::filesystem;

	// This class is meant to be used with a singleton
	class SILENT_API Resources
	{
	private:
		std::map<std::string, std::shared_ptr<Resource>> resources;
	public:

		bool ResourceLoaded(std::string str);

		template<typename T> 
		std::shared_ptr<T> GetResource(std::string str)
		{
			if (resources.count(str))
			{
				return std::dynamic_pointer_cast<T>(resources[str]);
			}

			std::cout << "No Resource of type " << typeid(T).name()
				<< " named " << str << "found\n";
			return nullptr;
		}

		template<typename T, typename... TArgs>
		std::shared_ptr<T> LoadResource(std::string str, TArgs&&... mArgs)
		{
			if (!ResourceLoaded(str))
			{
				T* c(new T(str, std::forward<TArgs>(mArgs)...));
				std::shared_ptr<Resource> sPtr{ c };
				resources[str] = sPtr;
				return std::dynamic_pointer_cast<T>(sPtr);
			}
			else
			{
				return GetResource<T>(str);
			}
		}

		void LoadAllResources();
		void Cleanup();
	};
}