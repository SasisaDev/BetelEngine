#include "Platform.h"

IPlatform* IPlatform::Get()
{
#	ifdef PLATFORM_WINDOWS
		static IPlatform* Platform = new WindowsPlatform();
#	elif defined(PLATFORM_LINUX)
		static IPlatform* Platform = new LinuxPlatform();
#	else
		static IPlatform* Platform = nullptr;
#	endif

	return Platform;
}