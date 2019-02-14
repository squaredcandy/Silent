#pragma once

#include "../Core.h"
#include "Stack/Stack.h"

namespace Silent
{
	class ApplicationStack
	{
	public:
		std::vector<std::unique_ptr<Stack>> stack;
		std::vector<std::unique_ptr<Stack>>::iterator stackEnd;

		ApplicationStack()
		{
			stackEnd = stack.end();
		}
		~ApplicationStack() = default;

		template<typename T, typename... TArgs>
		T * AddToStack(TArgs&&... mArgs)
		{
			T newStack(new Stack(std::forward<TArgs>(mArgs)...));
			std::unique_ptr<Stack> uPtr{ newStack };
			stack.emplace(stackEnd--, std::move(uPtr));
		}

		template<typename T, typename... TArgs>
		T * AddToStackEnd(TArgs&&... mArgs)
		{
			T newStack(new Stack(std::forward<TArgs>(mArgs)...));
			std::unique_ptr<Stack> uPtr{ newStack };
			stack.emplace(stackEnd, std::move(uPtr));
		}
	};
}