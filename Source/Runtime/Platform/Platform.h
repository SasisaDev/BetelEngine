#pragma once

#include "platformdefs.h"

#include "Interface/IPlatform.h"

#ifdef PLATFORM_WINDOWS
#	include "Windows/WindowsPlatform.h"
#elif defined(PLATFORM_LINUX)
#	include "Linux/LinuxPlatform.h"
#endif