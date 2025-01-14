#include "AssetLoader.h"

#include <Log/Logger.h>
#include <Platform/Platform.h>

#include <cstring>
#include <memory>

uint32_t ConvertChar::ToUInt32(char* buffer)
{
    return (uint32_t)buffer[3] << 24 |
           (uint32_t)buffer[2] << 16 |
           (uint32_t)buffer[1] << 8  |
           (uint32_t)buffer[0];
}

uint32_t ConvertChar::ToUInt16(char* buffer)
{
    return (uint32_t)buffer[1] << 8  |
           (uint32_t)buffer[0];
}

uint32_t ConvertChar::ToUInt8(char* buffer)
{
    return (uint32_t)buffer[0];
}

BlameMasterFile* AssetLoader::ParseBlameMasterFile(IFile* file)
{
#   define CHECKREAD(count) \
    if(buffer) \
        delete buffer; \
    buffer = file->Fetch(count); \
    if(buffer == nullptr) {\
        LOG(Error, LogAssetLoader, "Failed reading BMF file. EOF was not expected"); \
        delete BMF; \
        return nullptr;\
    }

    BlameMasterFile* BMF = new BlameMasterFile;
    BMF->FileHandle = file;
    BMF->SetName(file->GetPath().GetName()); 
    BMF->SetPath(file->GetPath().GetPath()); 
    BMF->SetMounted(true);

    // TODO: Fetch header
    char* buffer = nullptr;
    CHECKREAD(3);
    memcpy(BMF->FileHeader.pSignature, buffer, 3);
    if(BMF->FileHeader.pSignature[0] != 'B' || BMF->FileHeader.pSignature[1] != 'M' || BMF->FileHeader.pSignature[2] != 'F')
    {
        LOG(Error, LogAssetLoader, "Attempted reading BMF file, but it had wrong header signature");
        delete BMF;
        return nullptr;
    }

    CHECKREAD(4);
    BMF->FileHeader.uMagic = (uint32_t)buffer[0] << 24 |
                             (uint32_t)buffer[1] << 16 |
                             (uint32_t)buffer[2] << 8  |
                             (uint32_t)buffer[3];

    if(BMF->FileHeader.uMagic != BLAME_MASTER_FILE_MAGIC)
    {
        LOG(Error, LogAssetLoader, "Attempted reading BMF file, but it had wrong header magic");
        delete BMF;
        return nullptr;
    }
    
    CHECKREAD(2);
    BMF->FileHeader.uVersion = (uint16_t)buffer[0] << 8  |
                               (uint16_t)buffer[1];

    if(BMF->FileHeader.uVersion > BLAME_MASTER_FILE_VERSION)
    {
        LOGF(Error, LogAssetLoader, "Attempted reading BMF file, but it's version is newer than current engine version supports: %u > %u", BMF->FileHeader.uVersion, BLAME_MASTER_FILE_VERSION);
        delete BMF;
        return nullptr;
    }

    // TODO: Fetch table
    CHECKREAD(4);
    BMF->FileTable.uObjectCount = ConvertChar::ToUInt32(buffer);

    BMF->FileTable.pObjects = new BlameObjectTableEntry[BMF->FileTable.uObjectCount];

    for(int i = 0; i < BMF->FileTable.uObjectCount; ++i)
    {
        CHECKREAD(4);
        BMF->FileTable.pObjects[i].ID = ConvertChar::ToUInt32(buffer);

        CHECKREAD(4);
        BMF->FileTable.pObjects[i].offset = ConvertChar::ToUInt32(buffer);
    }

    return BMF;
#   undef CHECKREAD
}

void AssetLoader::CrawlContent(std::string Path)
{
    IDirectory* directory = IPlatform::Get()->OpenLocalDirectory(Path);

    if(!directory->Exists()) {
        LOGF(Error, LogAssetLoader, "Attempted to crawl directory \"%s\", but it doesn't exist.", Path.c_str());
        delete directory;
        return;
    }

    for(IDirectory* child : directory->GetChildren())
    {
        if(child->IsDirectory() == false)
        {
            // If file is a BMF file, parse it accoardingly
            std::string extension = child->GetPath().GetExt();
            if(extension == BLAME_MASTER_FILE_EXT) 
            {
                BlameMasterFile* BMF = ParseBlameMasterFile(IPlatform::Get()->OpenFile(child->GetPath(), FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY));
                if(BMF != nullptr) {
                    blameMasters.push_back(BMF);
                }
            } else if (extension == ARCHIVE_FILE_EXT)
            {
                // TODO: Parse Archive
            }
        }
    }

    delete directory;
}

BlameMasterFileObjectContainer AssetLoader::ReadObject(BlameMasterFile* master, uint32_t offset)
{
#   define CHECKREAD(count) \
    if(buffer) delete buffer; \
    buffer = master->FileHandle->Fetch(count); \
    if(buffer == nullptr) {\
        LOG(Error, LogAssetLoader, "Failed reading BMF file. EOF was not expected"); \
        return {0};\
    }

    char* buffer = nullptr;

    BlameMasterFileObjectContainer container;

    master->FileHandle->Seek(offset, FILE_SEEK_FLAG_BEG);

    CHECKREAD(4);
    container.uObjectSize = ConvertChar::ToUInt32(buffer);

    CHECKREAD(1);
    container.object.uClassNameLength = ConvertChar::ToUInt8(buffer);
    if(container.object.uClassNameLength == 0)
    {
        LOG(Error, LogAssetLoader, "Attempted reading BMF file object, but it's class name length is 0");
        return {0};
    }

    CHECKREAD(container.object.uClassNameLength);
    memmove(container.object.pClassName, buffer, container.object.uClassNameLength);

    return container;
#   undef CHECKREAD
}

Object* AssetLoader::LoadObject(uint32_t ObjectID)
{
    Object* object = nullptr;

    for(BlameMasterFile* master : blameMasters)
    {
        if(master->bIsMounted)
        {
            if(master->Table.contains(ObjectID))
            {
                BlameMasterFileObjectContainer container = ReadObject(master, master->Table[ObjectID].offset);
            }
        }
    }

    return object;
}

Resource* AssetLoader::LoadResource(std::string path)
{
    // TODO: Make Archive Loading

    IFile* file = IPlatform::Get()->OpenContentFile(path, FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);
    if(!file->IsOpen())
    {
        LOGF(Error, LogAssetLoader, "Attempted to load Resource \"%s\", but it doesn't exist", path.c_str());
        return nullptr;
    }

    Resource* resource = new Resource(file->FetchAllBinary());

    LOGF(Log, LogAssetLoader, "Loaded Resource \"%s\".", path.c_str());

    return resource;
}