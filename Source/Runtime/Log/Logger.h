#pragma once

#include <string>
#include <Delegate/Delegate.h>

class Logger {
protected:
	inline static void InternalLog(const char* Namespace, const char* message, const char* prefix = "");
public:
	static TMulticastDelegate<const char*, const char*, const char*> OnLogMessage;
	static TMulticastDelegate<const char*, const char*, const char*> OnLogFatal;

	static void Log(const char* Namespace, const char* message);
	static void Log(const char* Namespace, std::string message);
	static void LogFormat(const char* Namespace, const char* message, ...);
	static void LogFormat(const char* Namespace, std::string message, ...);

	static void Warning(const char* Namespace, const char* message);
	static void Warning(const char* Namespace, std::string message);
	static void WarningFormat(const char* Namespace, const char* message, ...);
	static void WarningFormat(const char* Namespace, std::string message, ...);

	static void Error(const char* Namespace, const char* message);
	static void Error(const char* Namespace, std::string message);
	static void ErrorFormat(const char* Namespace, const char* message, ...);
	static void ErrorFormat(const char* Namespace, std::string message, ...);

	static void Fatal(const char* Namespace, const char* message);
	static void Fatal(const char* Namespace, std::string message);
	static void FatalFormat(const char* Namespace, const char* message, ...);
	static void FatalFormat(const char* Namespace, std::string message, ...);
};

/* 
 * Sends message to the log
 *	
 * Type: Log, Warning, Error or Fatal
 */
#define LOG(Type, Namespace, Message) Logger:: Type ( #Namespace, Message)

 /*
  * Sends formated message to the log
  *
  * Type: Log, Warning, Error or Fatal
  */
#define LOGF(Type, Namespace, Message, ...) Logger:: Type##Format ( #Namespace, Message, __VA_ARGS__ )