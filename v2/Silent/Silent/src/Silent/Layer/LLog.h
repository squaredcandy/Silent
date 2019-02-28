#pragma once

#include "../Core.h"
#include "Layer.h"

namespace Silent
{
	// This is the logging layer that we use when we want to log something
	class SILENT_API LLog : public Layer
	{
	private:
		std::vector<std::string> _log;
	public:
		void AddLog(std::string newLog);
		void AddLog(const char * format, ...);
		const std::vector<std::string>& log;

		LLog();
		virtual ~LLog() = default;

		virtual void OnInitialize() override;
		virtual void OnUpdate() override;
		virtual void OnDestroy() override;
	};
}

#include "../Utility/Locator.h"
#ifdef _DEBUG
#define LOG(x, ...) Locator<LLog>::Get()->AddLog(x, __VA_ARGS__)
#else
#define LOG(x, ...)
#endif // _DEBUG