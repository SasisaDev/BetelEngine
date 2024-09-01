#include "WindowsPlatform.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#pragma comment(lib, "Kernel32.lib")

#include <iostream>
#include <ios>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fstream>
#include <fcntl.h>

#include "Files/WindowsFile.h"

bool WindowsPlatform::Win32RedirectConsole()
{
	/*FILE* fp;

	if (GetStdHandle(STD_OUTPUT_HANDLE) == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	if (freopen_s(&fp, "CONOUT$", "w", stdout) != 0)
	{
		return false;
	}

	setvbuf(stdout, NULL, _IONBF, 0);

	std::ios::sync_with_stdio(true);

	std::cout.clear();*/

	return true;
}

bool WindowsPlatform::Win32ReleaseConsole()
{
	
	FILE* fp;

	if (freopen_s(&fp, "NUL:", "w", stdout) != 0)
	{
		return false;
	}

	setvbuf(stdout, NULL, _IONBF, 0);

	if (!FreeConsole())
	{
		return false;
	}

	return true;
}

WindowsPlatform::WindowsPlatform()
{
	Win32RedirectConsole();
}

WindowsPlatform::~WindowsPlatform()
{
	Win32ReleaseConsole();
}

std::string WindowsPlatform::GetExecutablePath()
{
	// TODO: Implement

	return IPlatform::GetExecutablePath();
}

void WindowsPlatform::DebugPrint(const char* string) const
{
	//std::cout << string;
	//std::printf(string);
	//OutputDebugStringW(std::wstring(string, string+strlen(string)).c_str());
	OutputDebugStringA(string);
}

IFile* WindowsPlatform::OpenFile(const char* path, uint8_t accessFlags)
{
	return IPlatform::OpenFile(path, accessFlags);
	//return new WindowsFile(path, accessFlags);
}