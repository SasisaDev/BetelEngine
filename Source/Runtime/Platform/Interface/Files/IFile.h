#pragma once

#include <string>
#include <cstdint>
#include <fstream>
#include <vector>

enum EFileAccessFlags : uint8_t
{
	FILE_ACCESS_FLAG_READ = (1 << 0),
	FILE_ACCESS_FLAG_WRITE = (1 << 1),
	FILE_ACCESS_FLAG_BINARY = (1 << 2),
	FILE_ACCESS_FLAG_ATE = (1 << 3)
};

class IFile
{
	std::fstream file;
public:
	IFile() = default;
	IFile(std::string path, uint8_t accessFlags);
	~IFile();

	virtual const char* Fetch(size_t count);
	virtual std::string FetchAll();

	virtual std::vector<char> FetchAllBinary();

	virtual void Write(std::string content) { file << content; }
	virtual void Close() {file.close();}
};