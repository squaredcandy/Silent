#include "Resources.h"

namespace Silent::Engine::Resources
{
	std::map<std::string, std::shared_ptr<Resource>> resources;

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