#pragma once

#include <SDL.h>
#include <Log/Logger.h>

int GuardedMain()
{
	LOGF(Log, LogTest, "Unicode String: %s", "僕の Test String");

	return 0;
}