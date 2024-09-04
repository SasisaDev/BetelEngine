#pragma once

#include <string>
#include <vector>

// TODO: Make more robust path handling
struct IPath
{
    IPath(){isRelative = true;}
    IPath(const std::string& path);

    bool isRelative;

    void MakeAbsolute();
    void MakeRelative(std::string relTo);
    void MakeRelative(const IPath& relTo);

    std::string GetPath();
    inline char GetDisc() const {return disc;}

    // Add subdirectory
    const IPath& operator+(const IPath& path);
    const IPath& operator+(const std::string& path);
    // Go one directory back
    const IPath& operator--();

    // String decay
    operator std::string(){return GetPath();}
private:
    std::vector<std::string> pathMembers;
    char disc = '.';
};