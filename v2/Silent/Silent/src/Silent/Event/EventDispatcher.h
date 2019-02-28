#pragma once

#include "../Core.h"
#include "Events.h"

namespace Silent
{
	template<class T>
	struct CallbackStruct
	{
		using CallbackFn = std::function<bool(T&)>;
		std::string _name;
		CallbackFn _fn;
		CallbackStruct(std::string name = "", CallbackFn fn = [] (T& t) { return false; }) :
			_name(name), _fn(fn) {}
		~CallbackStruct() = default;
	};

	template<class T>
	class SILENT_API EventDispatcher
	{
	private:
		using CallbackFn = std::function<bool(T&)>;
		inline static std::vector<CallbackStruct<T>> callbacks;
	public:
		EventDispatcher() = default;
		~EventDispatcher()
		{
			callbacks.clear();
		}

		static std::string AddCallback(CallbackFn callbackFn, std::string name = "")
		{
			callbacks.emplace_back(CallbackStruct<T>(name, callbackFn));
			return name;
		}

		static std::string AddCallback(CallbackStruct<T> callback)
		{
			callbacks.emplace_back(callback);
			return callback._name;
		}

		static void Dispatch(T * event)
		{
			static_assert(std::is_base_of<Event, T>::value);
			for (const auto& c : callbacks)
			{
				c._fn(*event);
				if (event->handled)
				{
					break;
				}
			}
		}
		static void RemoveAllCallbacks()
		{
			callbacks.clear();
		}
	};
}