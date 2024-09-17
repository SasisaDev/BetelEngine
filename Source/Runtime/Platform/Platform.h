#pragma once

#include "platformdefs.h"

#include "Interface/IPlatform.h"
#include "Interface/Files/IFile.h"

#ifndef PLATFORM_WINDOWS
#   define __stdcall
#endif

#ifdef PLATFORM_WINDOWS
#	include "Windows/WindowsPlatform.h"
#elif defined(PLATFORM_LINUX)
#	include "Linux/LinuxPlatform.h"
#endif