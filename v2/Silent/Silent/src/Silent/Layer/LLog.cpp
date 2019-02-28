#include "LLog.h"
#include <mutex>

using namespace Silent;
std::mutex log_mutex;
std::mutex log_mutex_2;

LLog::LLog() : log(_log)
{
}
void LLog::OnInitialize()
{
}
void LLog::OnUpdate()
{
}
void LLog::OnDestroy()
{
}
void LLog::AddLog(std::string newLog)
{
	std::lock_guard<std::mutex> guard(log_mutex_2);
	_log.emplace_back(newLog);
}

void LLog::AddLog(const char * format, ...)
{
	std::lock_guard<std::mutex> guard(log_mutex);
	char buf[100];
	va_list args;
	va_start(args, format);
	vsprintf_s(buf, format, args);
	va_end(args);
	AddLog((std::string)buf);
}