#include "Resources.h"

namespace Silent
{
#define RESOURCE_MAP_ARGS std::string name, std::string path
	void LoadFBX(RESOURCE_MAP_ARGS)
	{

	}

	void LoadPNG(RESOURCE_MAP_ARGS)
	{

	}

	void ExtensionToLoaderMapper(fs::path& fp)
	{
		std::string ext = fp.extension().string();
		std::transform(ext.begin(), ext.end(), ext.begin(), ::toupper);
		std::string filename = fp.stem().string();
		std::string filepath = fp.string();
		//if (ext == ".PNG") LoadResource<Texture>(filename, filepath);
		//else if (ext == ".FBX") LoadResource<Mesh>(filename, filepath);
	}

	bool Resources::ResourceLoaded(std::string str)
	{
		return resources.count(str);
	}

	void Resources::LoadAllResources()
	{
		fs::directory_entry directory(DATA_FOLDER);
		if (!directory.exists())
		{
			//LOG_ERROR("%s does not exists", directory.path().string().c_str());
			return;
		}

		fs::path fp;
		//std::string fpString;
		//std::string extString;

		for (const auto& p : fs::recursive_directory_iterator(directory))
		{
			fp = p.path();
			//fpString = fp.string();
			//LOG_TRACE("%s", fpString.c_str());

			if (fp.has_extension())
			{
				ExtensionToLoaderMapper(fp);
				//extString = fp.extension().string();
				//std::transform(extString.begin(), extString.end(),
				//			   extString.begin(), ::toupper);
				//
				//LOG_TRACE("%s", extString.c_str());
				//
				//if (resourceLoadMap.count(extString))
				//{
				//	std::string filename = fp.stem().string();
				//	resourceLoadMap[extString](filename, fpString);
				//}
			}
		}
	}

	void Resources::Cleanup()
	{
		for (auto r : resources)
		{
			r.second->Cleanup();
		}
		resources.clear();
	}
}