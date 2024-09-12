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

enum EFileSeekFlags : uint8_t
{
	FILE_SEEK_FLAG_EOF,
	FILE_SEEK_FLAG_CUR,
	FILE_SEEK_FLAG_BEG
};


class IFile
{
	std::fstream file;
public:
	IFile() = default;
	IFile(std::string path, uint8_t accessFlags);
	~IFile();

	virtual bool IsOpen() const {return file.is_open();}

	virtual size_t Position() {return file.tellg();}
	virtual void Seek(size_t offset, uint8_t seekOrigin);
	virtual const char* Peek(size_t count);
	virtual const char* Fetch(size_t count);
	virtual std::string FetchAll();

	virtual std::vector<char> FetchAllBinary();

	virtual void Write(std::string content) { file << content; }
	virtual void Close() {file.close();}
};