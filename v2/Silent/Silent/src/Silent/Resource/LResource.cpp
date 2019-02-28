#include "LResource.h"
#include "../Layer/LLog.h"
#include "RBuffer.h"
#include <chrono>

using namespace Silent;
using namespace std::chrono_literals;

void LResource::OnInitialize()
{
	auto queuefunc = [] (Resource *) 
	{
		std::this_thread::sleep_for(1s);
		//LOG("rnd\n");
	};
	loader.QueueResource(queuefunc, nullptr);
	//loader.QueueResource(queuefunc, nullptr);
	//loader.QueueResource(queuefunc, nullptr);
	//loader.QueueResource(queuefunc, nullptr);
	//loader.QueueResource(queuefunc, nullptr);
	auto * r = Locator<LRenderer>::Get();
	LoadResource<RBuffer>("main", r);
	LoadResource<RBuffer>("main2", r);
	//LoadResource<RBuffer>("main3", r);
}
void LResource::OnUpdate()
{
	loader.Update();
}
void LResource::OnDestroy()
{
	for (auto& r : resources)
	{
		delete r;
	}

	ids.clear();
	names.clear();
	resources.clear();
}

void ResourceLoader::QueueResource(ResourceLoadFn fn, Resource * resource)
{
	loadQueue.emplace(std::bind(fn, resource));
}

void ResourceLoader::Update()
{
	// Clear up any finished threads
	for (int i = 0; i < MAX_THREADS; ++i)
	{
		if (!threadsAvailable[i])
		{
			auto status = threads[i].wait_for(0ms);
			if (status == std::future_status::ready)
			{
				threadsAvailable[i] = true;
			}
		}
	}

	if (loadQueue.empty()) return;

	// Assign new work for the threads
	for (int i = 0; i < MAX_THREADS; ++i)
	{
		if (threadsAvailable[i])
		{
			threads[i] = std::async(std::launch::async, loadQueue.front());
			threadsAvailable[i] = false;
			loadQueue.pop();
		}
		if (loadQueue.empty()) break;
	}
}