#include <Types/Types.h>

class IFile
{
public:

	virtual const char* Fetch(size_t count) { return nullptr; }
	virtual SString FetchAll() { return 0; }

	virtual void Write(SString content) {}
	virtual void Close() {}
};