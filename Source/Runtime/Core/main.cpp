extern int GuardedMain(int argc, char* argv[]);

#include <Platform/platformdefs.h>
#include <Platform/Platform.h>

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

int main(int argc, char* argv[])
{
	return GuardedMain(argc, argv);
}

int WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR     lpCmdLine,
		int       nShowCmd
)
{
	int argc;
	char** argv;

	wchar_t ** wargv = CommandLineToArgvW( GetCommandLineW(), &argc );
  	if (!wargv) 
	{ 
		argc = 0; argv = NULL;  
	}
	
  	int n = 0;
  	for (int i = 0;  i < argc;  i++)
	{
  		n += WideCharToMultiByte( CP_UTF8, 0, wargv[i], -1, NULL, 0, NULL, NULL ) + 1;
	}

  	argv = new char*[(argc + 1) * sizeof(char *) + n ];
  	if (!argv) 
	{ 
		argc = 0;
	}
	
  	char * arg = (char *)&((argv)[argc + 1]);
  	for (int i = 0;  i < argc;  i++)
  	{
  	  argv[i] = arg;
  	  arg += WideCharToMultiByte( CP_UTF8, 0, wargv[i], -1, arg, n, NULL, NULL ) + 1;
  	}

  	argv[argc] = NULL;

	// Get Executable Path
	char execFolderBuf[1024];
	size_t execFolderBufLen = sizeof(execFolderBuf); 

	int execFolderBytes = GetModuleFileName(NULL, execFolderBuf, execFolderBufLen);
	IPlatform::Get()->SetExecutableLocalPath(IPath(execFolderBuf).StepBack());

	return GuardedMain(argc, argv);
}
#else

int main(int argc, char* argv[])
{
	return GuardedMain(argc, argv);
}
#endif