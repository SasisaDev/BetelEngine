#pragma once

#include <string>
#include <cstdint>

enum class EFileAccessFlags : uint8_t
{
	Read = (1 << 0),
	Write = (1 << 1),
	Execute = (1 << 2)
};

class IFile
{
public:
	IFile() = default;
	IFile(std::string path, EFileAccessFlags accessFlags) {};

	virtual const char* Fetch(size_t count) { return nullptr; }
	virtual std::string FetchAll() { return 0; }

	virtual void Write(std::string content) {}
	virtual void Close() {}
};