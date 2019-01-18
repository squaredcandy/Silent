#include "Modules.h"
#include "Singleton.h"
#include "../Entity/Entities.h"

namespace Silent
{
	void Modules::Cleanup()
	{
		for (const auto& [key, val] : modules)
		{
			for (const auto& mod : val)
			{
				delete mod;
			}
		}
	}

	const std::map<std::type_index, std::pair<int, int>>& Modules::GetModifiedCounter()
	{
		return typesCounter;
	}

	void Modules::UpdateTypesModified()
	{
		if (!typesHandled.empty())
		{
			for (const auto& [key, val] : typesHandled)
			{
				typesModified[key] = false;
				typesCounter[key] = { 0, 0 };
			}
			typesHandled.clear();
		}
	}
}