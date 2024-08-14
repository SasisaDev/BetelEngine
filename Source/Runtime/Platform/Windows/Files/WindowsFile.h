#pragma once

#include <Platform/Interface/Files/IFile.h>

class WindowsFile : public IFile
{
public:
	WindowsFile() = delete;
	WindowsFile(std::string path, EFileAccessFlags accessFlags);

	virtual const char* Fetch(size_t count) override { return nullptr; }
	virtual std::string FetchAll() override { return 0; }

	virtual void Write(std::string content) override {}
	virtual void Close() override {}
};