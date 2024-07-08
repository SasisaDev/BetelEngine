extern int GuardedMain();

#include <Platform/platformdefs.h>

#ifdef PLATFORM_WINDOWS

#include <Windows.h>

int WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR     lpCmdLine,
		int       nShowCmd
)
{
	return GuardedMain();
}
#else

int main(int argc, char* argv[])
{
	return GuardedMain();
}
#endif