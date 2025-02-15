#pragma once

#include <string>
#include <vector>

// TODO: Make more robust path handling
struct IPath
{
    IPath(){isRelative = true;}
    IPath(const std::string& path);
    IPath(const char* path);

    bool isRelative;

    IPath& MakeAbsolute();
    IPath& MakeRelative(std::string relTo);
    IPath& MakeRelative(const IPath& relTo);

    std::string GetPath() const;
    inline char GetDisc() const {return disc;}
    std::string GetName() const;
    std::string GetNameExt() const;
    std::string GetExt() const;

    // Add subdirectory
    IPath operator+(const IPath& path);
    IPath operator+(const std::string& path);
    // Go one directory back
    IPath& operator--();
    IPath StepBack();

    // String decay
    operator std::string(){return GetPath();}
private:
    std::vector<std::string> pathMembers;
    char disc = '.';
};