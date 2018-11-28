#pragma once

#include <thread>
#include <vector>

namespace Silent::Engine::Multithread
{
	extern std::vector<std::unique_ptr<std::thread>> threadPool;

	template<class T, class... Arg>
	void CreateThread(std::string threadName,T&& t, Arg&&... args)
	{
		std::unique_ptr<std::thread> newThread = 
			std::make_unique<std::thread>(std::forward<T>(t), 
				std::forward<Arg>(args)...);
		
		newThread->detach();
		//threadPool.emplace_back(std::move(newThread));
	}

	void ManageThreads();
}