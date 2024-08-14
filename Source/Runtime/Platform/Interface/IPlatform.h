#pragma once

#include "Files/IFile.h"

class IPlatform
{
public:

	static IPlatform* Get();

	virtual const char* PlatformName() const { return "Unknown"; }
	virtual void DebugPrint(const char* string) const {}

	virtual IFile* OpenFile(const char* path, EFileAccessFlags accessFlags) { return nullptr; }
};