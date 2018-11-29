#include "Log.h"

#include "ImGui/imgui.h"

#include "Settings.h"

namespace Silent::Engine::Log
{
	std::vector<LogEntry> logHistory;
	void InitLog()
	{
		LOG_TRACE("Testing Trace Log");
		LOG_INFO("Testing Info Log");
		LOG_WARN("Testing Warn Log");
		LOG_ERROR("Testing Error Log");
		LOG_FATAL("Testing Fatal Log");
	}

	void AddToLog(InfoLevel lvl, const char * format, ...)
	{
		char buf[BUFFER_SIZE];
		va_list args;
		va_start(args, format);
		_vsnprintf_s(buf, BUFFER_SIZE, _TRUNCATE, format, args);
		va_end(args);

		logHistory.emplace_back(LogEntry(lvl, buf));
	}
}