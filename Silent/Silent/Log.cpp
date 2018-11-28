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

	void Draw()
	{
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		ImGui::Begin("Log", &Settings::showLog);
		ImGui::BeginChild("Scrolling", ImVec2(0, 0), false,
						  ImGuiWindowFlags_HorizontalScrollbar);
		for (const auto& entry : logHistory)
		{
			std::string info;
			// Up to here - Adding color to log
			switch (entry.infoLevel)
			{
			case TRACE_LEVEL:
				info = "[TRACE]";
				break;
			case INFO_LEVEL:
				info = "[INFO]";
				break;
			case WARN_LEVEL:
				info = "[WARN]";
				break;
			case ERROR_LEVEL:
				info = "[ERROR]";
				break;
			case FATAL_LEVEL:
				info = "[FATAL]";
				break;
			default:
				info = "[FAIL]";
				break;
			}

			ImGui::TextColored(Settings::infoColor[entry.infoLevel], 
							   "%s %s %s", entry.logTime.c_str(), info.c_str(), 
							   entry.msg.c_str());

			//ImGui::TextUnformatted(str.c_str());
		}
		ImGui::EndChild();
		ImGui::End();
	}
}