#include "IDirectory.h"
#include <filesystem>

IDirectory::IDirectory(IPath path, uint8_t flags)
{
    entry.isDirectory = true;
    entry.path = path;
    entry.exists = true;

    volatile auto npath = path.GetPath();

    if(!std::filesystem::exists(path.GetPath())) {
        if((flags & DIRECTORY_FLAG_CREATE) == DIRECTORY_FLAG_CREATE) {
            if(!std::filesystem::create_directories(path.GetPath()))
            {
                entry.exists = false;
            }
            return;
        } else {
            entry.exists = false;
        }
    }

    entry.nodes = ParseNested(path.GetPath(), (flags & DIRECTORY_FLAG_RECURSIVE) == DIRECTORY_FLAG_RECURSIVE);
}

IDirectory::IDirectory(const Entry& defaultEntry)
    : entry(defaultEntry)
{

}

const std::vector<IDirectory*>& IDirectory::GetChildren()
{
    if(nestedDirectories.size() == entry.nodes.size()){
        return nestedDirectories;
    }

    for(const Entry& e : entry.nodes) {
        if(e.isDirectory) {
        nestedDirectories.push_back(new IDirectory(e));
        }
    }

    return nestedDirectories;
}

const std::vector<IDirectory*>& IDirectory::FetchChildren()
{
    entry.nodes = ParseNested(entry.path.GetPath(), false);

    return GetChildren();
}

std::vector<IDirectory::Entry> IDirectory::ParseNested(std::string path, bool recursive)
{
    std::vector<IDirectory::Entry> nodes;

    std::filesystem::directory_iterator it(path);
    for (const auto& dirEntry : it) {
        Entry entry;

        entry.path = dirEntry.path().string();
        entry.isDirectory = dirEntry.is_directory();

        // Nested files
        if(entry.isDirectory && recursive) {
            entry.nodes = ParseNested(dirEntry.path().string(), recursive);
        }

        nodes.push_back(entry);
    }

    return nodes;
}