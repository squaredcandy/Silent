#include "Platforms.h"

#include <assert.h>

namespace Silent
{
	Platform * GetPlatform(int platformType)
	{
		if (platformType == SL_PLATFORM_SDL) return new Platform_SDL();
		return nullptr;
	}
}