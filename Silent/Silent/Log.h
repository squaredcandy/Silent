#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <memory>
#include <vector>
#include <string>

#define BUFFER_SIZE 256

enum InfoLevel
{
	TRACE_LEVEL,
	INFO_LEVEL,
	WARN_LEVEL,
	ERROR_LEVEL,
	FATAL_LEVEL
};

namespace Silent::Engine
{
	class Log
	{
	private:
		static std::vector<std::string> logHistory;
	public:
		static void InitLog();

		// Don't use this version, use the macro version underneath
		static void AddToLog(InfoLevel lvl, const char * format, ...);

		static void Draw();

		static std::vector<std::string>& History() { return logHistory; }
	};

}

#define LOG_TRACE(...) Silent::Engine::Log::AddToLog(TRACE_LEVEL, __VA_ARGS__)
#define LOG_INFO(...)  Silent::Engine::Log::AddToLog(INFO_LEVEL, __VA_ARGS__)
#define LOG_WARN(...)  Silent::Engine::Log::AddToLog(WARN_LEVEL, __VA_ARGS__)
#define LOG_ERROR(...) Silent::Engine::Log::AddToLog(ERROR_LEVEL, __VA_ARGS__)
#define LOG_FATAL(...) Silent::Engine::Log::AddToLog(FATAL_LEVEL, __VA_ARGS__)