#include "LinuxPlatform.h"
#include <iostream>
#include <ios>
#include <stdio.h>
#include <stdlib.h>
//#include <io.h>
#include <fstream>
#include <fcntl.h>

void LinuxPlatform::DebugPrint(const char* string) const
{
	std::cout << string << "\n";
}
