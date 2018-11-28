#include "Multithread.h"
#include "Log.h"
#include <algorithm>

namespace Silent::Engine::Multithread
{
	std::vector<std::unique_ptr<std::thread>> threadPool;

	void ManageThreads()
	{
		threadPool.erase(std::remove_if(threadPool.begin(), threadPool.end(),
			[](std::unique_ptr<std::thread>& thread) 
		{
			if (thread->joinable())
			{
				thread->join();
				LOG_INFO("Thread Joined");
				return true;
			}
			return false;
		}), threadPool.end());
	}
}