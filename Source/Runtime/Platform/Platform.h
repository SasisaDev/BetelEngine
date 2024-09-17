#pragma once

#include "platformdefs.h"

#include "Interface/IPlatform.h"
#include "Interface/Files/IFile.h"

#ifdef PLATFORM_WINDOWS
#	include "Windows/WindowsPlatform.h"
#elif defined(PLATFORM_LINUX)
#if defined(__GNUC__)
#	define __stdcall
#endif
#	include "Linux/LinuxPlatform.h"
#endif