#pragma once

#if defined(_WIN32) || defined(WIN32) || defined(__MINGW64__)
#   define PLATFORM_WINDOWS
#elif __unix__ // all unices, not all compilers
#   define PLATFORM_UNIX
#elif __linux__
#   define PLATFORM_LINUX
#elif __APPLE__
    // Mac OS, not sure if this is covered by __posix__ and/or __unix__ though...
#endif
