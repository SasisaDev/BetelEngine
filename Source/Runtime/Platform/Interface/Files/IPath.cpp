#include "IPath.h"

IPath::IPath(const std::string& path)
    : isRelative(false)
{
    size_t offset = 0;

    if(path.starts_with("./")) {
        offset = 2;
        isRelative = true;
    } else if(path.size() > 1 && path[1] == ':') {
        disc = path[0];
        offset = 3;
    }

    std::string member;
    for(int i = offset; i < path.size(); ++i) {
        if(path[i] == '/' || path[i] == '\\') {
            if(i - offset > 0) {
                pathMembers.push_back(path.substr(offset, i - offset));
            }

            offset = i + 1;
        }

        if((i == (path.size() - 1)) && path[i] != '/' && path[i] != '\\') {
            pathMembers.push_back(path.substr(offset, i - offset + 1));
        }
    }
}

std::string IPath::GetPath()
{
    std::string path;
    
    if(isRelative) {
        path += "./";
    } else {
        path.push_back(disc);
        path.append(":/");
    }

    for(size_t i = 0; i < pathMembers.size(); ++i) {
        path += pathMembers[i];
        if(i != pathMembers.size() - 1) {
            path += '/';
        }
    }

    return path;
}

const IPath& IPath::operator+(const IPath& path)
{
    pathMembers.insert(pathMembers.end(), path.pathMembers.begin(), path.pathMembers.end());
    return *this;
}

const IPath& IPath::operator+(const std::string& path)
{
    return this->operator+(IPath(path));
}

const IPath& IPath::operator--()
{
    pathMembers.pop_back();
    return *this;
}