#include "Logger.h"
#include <iostream>
#include <sstream>
#include <chrono>
#include <ctime>
#include <stdexcept>

#include <Platform/Platform.h>
#include <cstdarg>

MulticastDelegate<const char*, const char*, const char*> Logger::OnLogMessage = MulticastDelegate<const char*, const char*, const char*>();

void Logger::InternalLog(const char* Namespace, const char* message, const char* prefix)
{
	std::stringstream prefixStream;
	prefixStream << prefix << " [" << "HH:MM:SS:MS" << "] [" << Namespace << "] " << message << "\n\0";

	IPlatform::Get()->DebugPrint(prefixStream.str().c_str());
}

void Logger::Log(const char* Namespace, const char* message)
{
	OnLogMessage.Broadcast("Log", Namespace, message);

	InternalLog(Namespace, message);
}

void Logger::LogFormat(const char* Namespace, const char* message, ...)
{
	va_list varg_ptr;
	va_start(varg_ptr, message);

	const unsigned int bufferSize = vsnprintf(NULL, 0, message, varg_ptr) + 1;
	char* buffer = new char[bufferSize];

	vsprintf(buffer, message, varg_ptr);

	OnLogMessage.Broadcast("Log", Namespace, buffer);

	InternalLog(Namespace, buffer);

	va_end(varg_ptr);
}

void Logger::Warning(const char* Namespace, const char* message)
{
	OnLogMessage.Broadcast("Warning", Namespace, message);

	InternalLog(Namespace, message, "\x1B[33m");
}

void Logger::WarningFormat(const char* Namespace, const char* message, ...)
{
	OnLogMessage.Broadcast("Warning", Namespace, message);
}

void Logger::Error(const char* Namespace, const char* message)
{
	OnLogMessage.Broadcast("Error", Namespace, message);

	InternalLog(Namespace, message);
}

void Logger::ErrorFormat(const char* Namespace, const char* message, ...)
{
	OnLogMessage.Broadcast("Error", Namespace, message);
}

void Logger::Fatal(const char* Namespace, const char* message)
{
	OnLogMessage.Broadcast("Fatal", Namespace, message);

	InternalLog(Namespace, message);

	throw std::runtime_error(message);
}

void Logger::FatalFormat(const char* Namespace, const char* message, ...)
{
	OnLogMessage.Broadcast("Fatal", Namespace, message);
	
}
