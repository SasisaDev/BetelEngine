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
    } else {
        isRelative = true;
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

IPath::IPath(const char* path)
    :IPath(std::string(path))
{

}

// FIXME: This will return substring before the first dot, which may be incorrect for files with multiple level extensions
// For example "Filename.sub.ext"
std::string IPath::GetName() const {
    if(pathMembers.size() <= 0) {
        return "";
    }
    
    std::string member = pathMembers[pathMembers.size()-1];
    size_t tokenPos = member.find('.');

    return member.substr(0, tokenPos);
}

std::string IPath::GetNameExt() const {
    if(pathMembers.size() <= 0) {
        return "";
    }
    
    return pathMembers[pathMembers.size()-1];
}

std::string IPath::GetExt() const {
    if(pathMembers.size() <= 0) {
        return "";
    }
    
    std::string member = pathMembers[pathMembers.size()-1];
    size_t tokenPos = member.find('.');

    return member.substr(tokenPos+1, member.size()-(tokenPos + 1));
}

std::string IPath::GetPath() const
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

IPath IPath::operator+(const IPath& path)
{
    IPath nPath(*this);
    nPath.pathMembers.insert(nPath.pathMembers.end(), path.pathMembers.begin(), path.pathMembers.end());
    return nPath;
}

IPath IPath::operator+(const std::string& path)
{
    return this->operator+(IPath(path));
}

IPath& IPath::operator--()
{
    pathMembers.pop_back();
    return *this;
}

IPath IPath::StepBack()
{
    IPath nPath(*this);
    return --nPath;
}