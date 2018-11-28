#pragma once

#include "Mesh.h"
#include "Texture.h"

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace Silent::Engine::Resources
{
	extern std::map<std::string, std::shared_ptr<Resource>> resources;

	void LoadAllResources();
	bool ResourceLoaded(std::string str);
	
	template<typename T> T* GetResource(std::string str)
	{
		if (resources.count(str))
		{
			return (T*)&*resources[str];
		}
		
		std::cout << "No Resource of type " << typeid(T).name()
			<< " named " << str << "found\n";
		return nullptr;
	}

	template<typename T, typename... TArgs> 
	T* LoadResource(std::string str, TArgs&&... mArgs)
	{
		if (!ResourceLoaded(str))
		{
			T* c(new T(std::forward<TArgs>(mArgs)...));
			std::shared_ptr<Resource> sPtr{ c };
			resources[str] = sPtr;
			return c;
		}
		else
		{
			return GetResource<T>(str);
		}
	}

	void CleanupResources();
}