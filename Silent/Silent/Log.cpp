#include "Log.h"

#include "ImGui/imgui.h"

namespace Silent::Engine
{
	void Log::InitLog()
	{
		LOG_TRACE("hey this actually works %d", 1);
	}

	void Log::AddToLog(InfoLevel lvl, const char * format, ...)
	{
		char buf[BUFFER_SIZE];
		va_list args;
		va_start(args, format);
		_vsnprintf_s(buf, BUFFER_SIZE, _TRUNCATE, format, args);
		va_end(args);

		// maybe put the time here as well someday
		std::string info;

		switch (lvl)
		{
		case TRACE_LEVEL:
			info = "[TRACE] ";
			break;
		case INFO_LEVEL:
			info = "[INFO] ";
			break;
		case WARN_LEVEL:
			info = "[WARN] ";
			break;
		case ERROR_LEVEL:
			info = "[ERROR] ";
			break;
		case FATAL_LEVEL:
			info = "[FATAL] ";
			break;
		default:
			info = "[FAIL] ";
			break;
		}

		logHistory.emplace_back(info + buf);
	}

	void Log::Draw()
	{
		bool a = true;
		ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
		ImGui::Begin("Console", &a);
		ImGui::BeginChild("Scrolling", ImVec2(0, 0), false,
						  ImGuiWindowFlags_HorizontalScrollbar);
		for (const auto& str : logHistory)
		{
			// Up to here - Adding color to log
			ImGui::TextUnformatted(str.c_str());
		}
		ImGui::EndChild();
		ImGui::End();
	}

}