#include "Resources.h"

#include "Log.h"
#include <functional>

namespace Silent::Engine::Resources
{
	std::map<std::string, std::shared_ptr<Resource>> resources;

	namespace
	{
#define RESOURCE_MAP_ARGS std::string name, std::string path
		void LoadMeshResource(RESOURCE_MAP_ARGS)
		{
			LoadResource<Mesh>(name, path);
		}

		void LoadTextureResource(RESOURCE_MAP_ARGS)
		{
			LoadResource<Texture>(name, path);
		}

		// Has to be in this order
		std::map<std::string, std::function<void(std::string, std::string)>> 
			resourceLoadMap = {
			{ ".FBX", &LoadMeshResource },
			{ ".PNG", &LoadTextureResource }
		};
	}

	void LoadAllResources()
	{
		fs::directory_entry directory(DATA_FOLDER);
		if (!directory.exists())
		{
			LOG_ERROR("%s does not exists", directory.path().string().c_str());
			return;
		}

		fs::path fp;
		std::string fpString;
		std::string extString;

		for (const auto& p : fs::recursive_directory_iterator(directory))
		{
			fp = p.path();
			fpString = fp.string();
			LOG_TRACE("%s", fpString.c_str());

			if (fp.has_extension())
			{
				extString = fp.extension().string();
				std::transform(extString.begin(), extString.end(), 
							   extString.begin(), ::toupper);

				LOG_TRACE("%s", extString.c_str());
				
				if (resourceLoadMap.count(extString))
				{
					std::string filename = fp.stem().string();
					resourceLoadMap[extString](filename, fpString);
				}
			}
		}
	}

	bool ResourceLoaded(std::string str)
	{
		return resources.count(str);
	}

	void CleanupResources()
	{
		for (auto r : resources)
		{
			r.second->Cleanup();
		}
	}
}