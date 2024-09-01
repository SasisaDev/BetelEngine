#pragma once

#include "Files/IFile.h"

#include <string>
#include <vector>

class IPlatform
{
	std::vector<std::string> ExecVariables;
public:

	static IPlatform* Get();

	virtual void SetExecVariables(std::vector<std::string> newVars){ExecVariables = newVars;}
	virtual std::vector<std::string> GetExecVariables() const {return ExecVariables;}

	virtual std::string GetExecutablePath() { return (ExecVariables.size() > 0) ? ExecVariables[0] : ""; }

	virtual const char* PlatformName() const { return "Unknown"; }
	virtual void DebugPrint(const char* string) const {}

	virtual IFile* OpenFile(const char* path, uint8_t accessFlags) { return new IFile(path, accessFlags); }
};