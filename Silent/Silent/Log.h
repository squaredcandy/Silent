#pragma once

#include <stdarg.h>
#include <stdio.h>
#include <memory>
#include <vector>
#include <string>
#include <chrono>

#define BUFFER_SIZE 256

enum InfoLevel
{
	TRACE_LEVEL = 0,
	INFO_LEVEL  = 1,
	WARN_LEVEL  = 2,
	ERROR_LEVEL = 3,
	FATAL_LEVEL = 4,
};

#ifndef LOG_ENTRY_CLASS
#define LOG_ENTRY_CLASS

#include <time.h>
#define TIMESTAMP_LEN 32

// We do this so we can add more data into our log if nessessary
struct LogEntry
{
	std::string logTime;
	InfoLevel infoLevel;
	std::string msg;

	void LogTime()
	{
		time_t rawtime;
		struct tm timeInfo;
		time(&rawtime);
		localtime_s(&timeInfo, &rawtime);
		char timestamp[TIMESTAMP_LEN];
		strftime(timestamp, TIMESTAMP_LEN, "%T", &timeInfo);
		logTime = timestamp;
	}

	LogEntry()
	{
		infoLevel = FATAL_LEVEL;
		msg = "FAILED TO ADD MSG";
		LogTime();
	}

	LogEntry(InfoLevel infoLvl, std::string str) : 
		infoLevel(infoLvl), msg(str) 
	{
		LogTime();
	}

	virtual ~LogEntry() = default;
};
#endif

namespace Silent::Engine::Log
{
	extern std::vector<LogEntry> logHistory;
	void InitLog();

	// Don't use this version, use the macro version underneath
	void AddToLog(InfoLevel lvl, const char * format, ...);
}

#ifdef _DEBUG
#define LOG_TRACE(...) Silent::Engine::Log::AddToLog(TRACE_LEVEL, __VA_ARGS__)
#define LOG_INFO(...)  Silent::Engine::Log::AddToLog(INFO_LEVEL, __VA_ARGS__)
#define LOG_WARN(...)  Silent::Engine::Log::AddToLog(WARN_LEVEL, __VA_ARGS__)
#define LOG_ERROR(...) Silent::Engine::Log::AddToLog(ERROR_LEVEL, __VA_ARGS__)
#define LOG_FATAL(...) Silent::Engine::Log::AddToLog(FATAL_LEVEL, __VA_ARGS__)
#else
#define LOG_TRACE(...) 
#define LOG_INFO(...)  
#define LOG_WARN(...)  
#define LOG_ERROR(...) 
#define LOG_FATAL(...) 
#endif // _DEBUG