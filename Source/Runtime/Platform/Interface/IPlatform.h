#pragma once

#include "Files/IFile.h"
#include "Files/IDirectory.h"

#include <string>
#include <vector>
#include <map>

class IPlatform
{
protected:
	// Returns domain
	std::string Internal_SeparateLocalPathDomain(std::string in, std::string* purePath);
protected:
	std::vector<std::string> ExecVariables;
	IPath ExecutableLocalPath = "./";
public:

	static IPlatform* Get();

	virtual void SetExecVariables(std::vector<std::string> newVars){ExecVariables = newVars;}
	virtual std::vector<std::string> GetExecVariables() const {return ExecVariables;}

	// TODO: Rewrite it more neatly
	virtual std::string GetExecutablePath() { return (ExecVariables.size() > 0) ? ExecVariables[0] : ""; }
	virtual std::string GetExecutableFolder() { return (ExecVariables.size() > 0) ? IPath(ExecVariables[0]).StepBack() : ""; }

	virtual const char* PlatformName() const { return "Unknown"; }
	virtual void DebugPrint(const char* string) const {}

	virtual IFile* OpenFile(IPath path, uint8_t accessFlags) { return new IFile(path, accessFlags); }
	virtual IDirectory* OpenDirectory(IPath path, uint8_t flags = 0) { return new IDirectory(IPath(path), flags); }

	/*
     * If file is found, return IFile*, and nullptr if not 
     * Same Root folder as executable's folder.
     * Usage: OpenLocalFile("Content/Path/To/File")
     */
    virtual IFile* OpenLocalFile(std::string path, uint8_t flags = EFileAccessFlags::FILE_ACCESS_FLAG_READ);

    /*
     * If directory is found, return IDirectory*, and nullptr if not 
     * Same Root folder as executable's folder.
     * Usage: OpenLocalDirectory("Content/Path/To/Directory/")
     */
    virtual IDirectory* OpenLocalDirectory(std::string path, uint8_t flags = 0);

    /*
     * If file is found, return IFile*, and nullptr if not 
     * Usage: OpenContentFile("Path/To/File")
     */
    virtual IFile* OpenContentFile(std::string path, uint8_t flags = EFileAccessFlags::FILE_ACCESS_FLAG_READ);

    /*
     * If directory is found, return IDirectory*, and nullptr if not 
     * Usage: OpenContentDirectory("Path/To/Directory/")
     */
    virtual IDirectory* OpenContentDirectory(std::string path, uint8_t flags = 0);

    /*
     * Tries to fetch requested file from all known path aliases
     * Doesn't take alias.
     * Usage: FetchLocalFiles("Content/Path/To/File")
     */
    virtual std::vector<IFile*> FetchLocalFiles(std::string path, uint8_t flags = EFileAccessFlags::FILE_ACCESS_FLAG_READ) {return {};}
    
    /*
     * Tries to fetch requested directory from all known path aliases
     * Doesn't take alias.
     * Usage: FetchLocalDirectories("Content/Path/To/Directory/")
     */
    virtual std::vector<IDirectory*> FetchLocalDirectories(std::string path, uint8_t flags = 0) {return {};}
};