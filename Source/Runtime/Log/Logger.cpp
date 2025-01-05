#include "Logger.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <stdexcept>

#include <Platform/Platform.h>
#include <cstdarg>
#include <ctime>

MulticastDelegate<const char*, const char*, const char*> Logger::OnLogMessage = MulticastDelegate<const char*, const char*, const char*>();

std::string Logger::InternalGetTimeStamp()
{
    static std::time_t time = std::time(0);
    static std::tm timeOld = *std::localtime(&time);
    static std::string buffer = {};

    time = std::time(0);
    std::tm* timeNow = std::localtime(&time);

    if(timeOld.tm_hour == timeNow->tm_hour &&  timeOld.tm_sec == timeNow->tm_sec && timeOld.tm_min == timeNow->tm_min && buffer.size()) {
        return buffer;
    }

	timeOld = *timeNow;

	buffer.resize(7 + (timeNow->tm_hour > 9));
	sprintf(buffer.data(), "%d:%02d:%02d", timeNow->tm_hour, timeNow->tm_min, timeNow->tm_sec);

    return buffer;
}

void Logger::InternalLog(const char* Namespace, const char* message, const char* prefix)
{
	std::stringstream prefixStream;
	std::string timeStamp = InternalGetTimeStamp();
	prefixStream << prefix << " [" << timeStamp << "] [" << Namespace << "] " << message << "\n\0";

	OnLogMessage.Broadcast(timeStamp.c_str(), Namespace, message);

	IPlatform::Get()->DebugPrint(prefixStream.str().c_str());
}

void Logger::Log(const char* Namespace, const char* message)
{
	InternalLog(Namespace, message);
}

void Logger::LogFormat(const char* Namespace, const char* message, ...)
{
	va_list varg_ptr;
	va_start(varg_ptr, message);

	const unsigned int bufferSize = vsnprintf(NULL, 0, message, varg_ptr) + 1;
	char* buffer = new char[bufferSize];

	vsprintf(buffer, message, varg_ptr);

	InternalLog(Namespace, buffer);

	va_end(varg_ptr);
}

void Logger::Warning(const char* Namespace, const char* message)
{
	InternalLog(Namespace, message, "\x1B[33m");
}

void Logger::WarningFormat(const char* Namespace, const char* message, ...)
{
	va_list varg_ptr;
	va_start(varg_ptr, message);

	const unsigned int bufferSize = vsnprintf(NULL, 0, message, varg_ptr) + 1;
	char* buffer = new char[bufferSize];

	vsprintf(buffer, message, varg_ptr);

	InternalLog(Namespace, buffer, "\x1B[33m");

	va_end(varg_ptr);
}

void Logger::Error(const char* Namespace, const char* message)
{
	InternalLog(Namespace, message, "\x1B[31m");
}

void Logger::ErrorFormat(const char* Namespace, const char* message, ...)
{
	va_list varg_ptr;
	va_start(varg_ptr, message);

	const unsigned int bufferSize = vsnprintf(NULL, 0, message, varg_ptr) + 1;
	char* buffer = new char[bufferSize];

	vsprintf(buffer, message, varg_ptr);

	InternalLog(Namespace, buffer, "\x1B[31m");

	va_end(varg_ptr);
}

void Logger::Fatal(const char* Namespace, const char* message)
{
	InternalLog(Namespace, message, "\x1B[31m");

	throw std::runtime_error(message);
}

void Logger::FatalFormat(const char* Namespace, const char* message, ...)
{
	OnLogMessage.Broadcast("Fatal", Namespace, message);
}
