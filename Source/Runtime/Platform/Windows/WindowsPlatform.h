#pragma once

#include "../Interface/IPlatform.h"

class WindowsPlatform : public IPlatform
{
private:
	bool Win32RedirectConsole();
	bool Win32ReleaseConsole();
public:
	WindowsPlatform();
	~WindowsPlatform();

	virtual std::string GetExecutablePath() override;

	virtual void DebugPrint(const char* string) const override;

	virtual IFile* OpenFile(const char* path, EFileAccessFlags accessFlags) override;
};