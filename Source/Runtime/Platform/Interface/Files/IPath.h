#pragma once

#include <string>
#include <vector>

struct IPath
{
    IPath(const std::string& path);

    bool isRelative;

    void MakeAbsolute();
    void MakeRelative(std::string relTo);
    void MakeRelative(const IPath& relTo);

    std::string GetPath();

    // Add subdirectory
    const IPath& operator+(const IPath& member);
    // Go one directory back
    const IPath& operator--();
private:
    std::vector<std::string> pathMembers;
};