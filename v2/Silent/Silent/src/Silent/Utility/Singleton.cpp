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
		// We can use unique ptr here but our msvc memory leak checker false
		// positives this so instead we use a raw pointer and delete it ourselves
		//static std::unique_ptr<T> instance = std::make_unique<T>();
		//static std::shared_ptr<T> instance = std::make_shared<T>();
		static T * instance = new T();
		return *instance;
	}

//	template<class T>
//	std::shared_ptr<T>& Singleton<T>::InstanceS(bool destroy)
//	{
//		// We can use unique ptr here but our msvc memory leak checker false
//		// positives this so instead we use a raw pointer and delete it ourselves
//		//static std::unique_ptr<T> instance = std::make_unique<T>();
//		static std::shared_ptr<T> instance = std::make_shared<T>();
//
//		//static T * instance = new T();
//		return instance;
//	}

	// Have to force its definition here so we can use it across the dll border
	void Instance_Resources()
	{
		Singleton<Resources>::Instance();
		//Singleton<Resources>::InstanceS();
	}
	void Instance_Entities()
	{
		Singleton<Entities>::Instance();
		//Singleton<Entities>::InstanceS();
	}
	void Instance_Modules()
	{
		Singleton<Modules>::Instance();
		//Singleton<Modules>::InstanceS();
	}
	void Instance_Systems()
	{
		Singleton<Systems>::Instance();
		//Singleton<Systems>::InstanceS();
	}
}