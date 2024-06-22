extern int GuardedMain();

#ifdef _WIN32

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