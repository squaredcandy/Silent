#include "Singleton.h"

#include "../Resource/Resources.h"
#include "../Entity/Entities.h"
#include "../Module/Modules.h"
#include "../System/Systems.h"

namespace Silent
{
	template<class T>
	T& Singleton<T>::Instance()
	{
		static std::unique_ptr<T> instance = std::make_unique<T>();
		return *instance;
	}

	// Have to force its definition here so we can use it across the dll border
	void Instance_Resources()
	{
		Singleton<Resources>::Instance();
	}
	void Instance_Entities()
	{
		Singleton<Entities>::Instance();
	}
	void Instance_Modules()
	{
		Singleton<Modules>::Instance();
	}
	void Instance_Systems()
	{
		Singleton<Systems>::Instance();
	}
}