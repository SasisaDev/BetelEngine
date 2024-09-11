#pragma once

#include "Files/IFile.h"
#include "Files/IDirectory.h"

#include <string>
#include <vector>
#include <map>

class IPlatform
{
	std::vector<std::string> ExecVariables;
	std::map<std::string, IPath> PathAliases;
public:

	static IPlatform* Get();

	virtual void SetExecVariables(std::vector<std::string> newVars){ExecVariables = newVars;}
	virtual std::vector<std::string> GetExecVariables() const {return ExecVariables;}

	virtual std::string GetExecutablePath() { return (ExecVariables.size() > 0) ? ExecVariables[0] : ""; }

	virtual const char* PlatformName() const { return "Unknown"; }
	virtual void DebugPrint(const char* string) const {}

	virtual IFile* OpenFile(const char* path, uint8_t accessFlags) { return new IFile(path, accessFlags); }
	virtual IDirectory* OpenDirectory(const char* path, uint8_t flags = 0) { return new IDirectory(IPath(path), flags); }

	virtual void AddLocalPath(std::string localPath, IPath globalPath){PathAliases.insert_or_assign(localPath, globalPath);}

	    /*
     * If file is found, return IFile*, and nullptr if not 
     * Takes aliased path.
     * Usage: OpenLocalFile("Editor/Content/Path/To/File")
     */
    virtual IFile* OpenLocalFile(std::string path, EFileAccessFlags flags = EFileAccessFlags::FILE_ACCESS_FLAG_READ) {return nullptr;}

    /*
     * If directory is found, return IDirectory*, and nullptr if not 
     * Takes aliased path.
     * Usage: OpenLocalDirectory("Editor/Content/Path/To/Directory/")
     */
    virtual IDirectory* OpenLocalDirectory(std::string path, EDirectoryFlags flags = (EDirectoryFlags)0) {return nullptr;}

    /*
     * Tries to fetch requested file from all known path aliases
     * Doesn't take alias.
     * Usage: FetchLocalFiles("Content/Path/To/File")
     */
    virtual std::vector<IFile*> FetchLocalFiles(std::string path, EFileAccessFlags flags = EFileAccessFlags::FILE_ACCESS_FLAG_READ) {return {};}
    
    /*
     * Tries to fetch requested directory from all known path aliases
     * Doesn't take alias.
     * Usage: FetchLocalDirectories("Content/Path/To/Directory/")
     */
    virtual std::vector<IDirectory*> FetchLocalDirectories(std::string path, EDirectoryFlags flags = (EDirectoryFlags)0) {return {};}

    virtual std::vector<std::string> GetAliases(){return {};}
};