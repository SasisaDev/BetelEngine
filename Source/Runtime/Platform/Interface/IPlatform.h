#pragma once

class IFile;

class IPlatform
{
public:

	static IPlatform* Get();

	virtual const char* PlatformName() const { return "Unknown"; }
	virtual void DebugPrint(const char* string) const {}

	virtual IFile* OpenFile(const char* path, const char* access) { return nullptr; }
};