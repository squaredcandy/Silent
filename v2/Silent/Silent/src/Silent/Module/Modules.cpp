#include "Modules.h"

namespace Silent
{

	void Modules::UpdateTypesModified()
	{
		if (!typesHandled.empty())
		{
			for (const auto& [key, val] : typesHandled)
			{
				typesModified[key] = false;
			}
			typesHandled.clear();
		}
	}

}