#pragma once
#include "../Core.h"
#include "../Layer/Layer.h"
#include "Resource.h"

#include "../Layer/LLog.h"

#include <thread>
#include <future>
#include <functional>
#include <array>
#include <queue>

#define MAX_THREADS 1

namespace Silent
{
	using ResourceLoadFn = std::function<void(Resource *)>;
	using queueFn = std::_Binder<std::_Unforced, ResourceLoadFn&, Resource *&>;
	class SILENT_API ResourceLoader
	{
	private:
		std::array<bool, MAX_THREADS> threadsAvailable;
		std::array<std::future<void>, MAX_THREADS> threads;
		std::queue<queueFn> loadQueue;

	public:
		ResourceLoader()
		{
			threadsAvailable.fill(true);
		}
		~ResourceLoader() = default;

		void QueueResource(ResourceLoadFn fn, Resource * resource);
		void Update();
	};

	class SILENT_API LResource : public Layer
	{
	protected:
		ResourceLoader loader;
		std::vector<ResourceID> ids;
		std::vector<std::string> names;
		std::vector<Resource*> resources;
		//std::mutex test_m;

	public:
		LResource() = default;
		~LResource() = default;

		template<typename T, typename... TArgs>
		void LoadResource(std::string name, TArgs&&... mArgs);

		inline void OnInitialize() override;
		inline void OnUpdate() override;
		inline void OnDestroy() override;
	};

	template<typename T, typename... TArgs>
	void LResource::LoadResource(std::string name, TArgs&&... mArgs)
	{
		auto * newResource = new T(name, std::forward<TArgs>(mArgs)...);
		ids.emplace_back(newResource->id);
		names.emplace_back(name);
		resources.emplace_back(newResource);
		// This allows us to call our member function
		auto fn = [&] (Resource * resource) 
		{
			//std::lock_guard<std::mutex> guard(test_m);
			LOG("locked %s\n", resource->_name.c_str());
			resource->Load();
			LOG("unlocked %s\n", resource->_name.c_str());
		};
		loader.QueueResource(fn, newResource);
	}

}