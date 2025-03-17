#include "WindowsPlatform.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <commdlg.h>

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
	char pBuf[1024];
	size_t len = sizeof(pBuf); 

	int bytes = GetModuleFileName(NULL, pBuf, len);
	return (bytes ? std::string(pBuf) : IPlatform::GetExecutablePath());
}

void WindowsPlatform::DebugPrint(const char* string) const
{
	//std::cout << string;
	//std::printf(string);
	//OutputDebugStringW(std::wstring(string, string+strlen(string)).c_str());
	OutputDebugStringA(string);
}

void WindowsPlatform::ShowMessageWindow(const std::string_view& name, const char* content) const
{
	MessageBox(NULL, content, name.data(), MB_OK);
}

std::string WindowsPlatform::OpenOpenFileDialog(const FileDialogInfo &info)
{
	OPENFILENAME ofn;
	TCHAR szFile[260] = { };

	std::string filter;
	if(info.extensions.size() == 0) {
		filter = "All\0*.*\0";
	} else {
		for(auto ext : info.extensions){
			filter += ext.first;
			filter.push_back('\0');
			filter += ext.second;
			filter.push_back('\0');
		}
	}

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetWindow(NULL, 0);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter.data();
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	std::string execPath = GetExecutableFolder();
	ofn.lpstrInitialDir = execPath.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
	    return ofn.lpstrFile;
	}
	return ofn.lpstrFile;
}
