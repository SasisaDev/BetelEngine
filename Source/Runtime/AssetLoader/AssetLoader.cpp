#include "AssetLoader.h"

#include <Log/Logger.h>
#include <Platform/Platform.h>
#include <Object/ObjectTypeLibrary.h>
#include <Engine/Engine.h>
#include <Object/Object.h>

#include <cstring>
#include <memory>

float ConvertChar::ToFloat(char* buffer)
{
    return *reinterpret_cast<float*>(buffer);
}

int32_t ConvertChar::ToInt32(char* buffer)
{
    return (int32_t)buffer[3] << 24 |
           (int32_t)buffer[2] << 16 |
           (int32_t)buffer[1] << 8  |
           (int32_t)buffer[0];
}

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

BlameMasterFile* AssetLoader::ParseBlameMasterFile(std::unique_ptr<IFile> file)
{
#   define CHECKREAD(count) \
    buffer = BMF->FileHandle->Fetch(count); \
    if(buffer == nullptr) {\
        LOG(Error, LogAssetLoader, "Failed reading BMF file. EOF was not expected"); \
        delete BMF; \
        return nullptr;\
    }

    BlameMasterFile* BMF = new BlameMasterFile;
    BMF->FileHandle = std::move(file);
    BMF->SetName(BMF->FileHandle->GetPath().GetName()); 
    BMF->SetPath(BMF->FileHandle->GetPath().GetPath()); 
    BMF->SetMounted(true);

    // Fetch header
    char* buffer = nullptr;
    CHECKREAD(3);
    memcpy(BMF->FileHeader.pSignature, buffer, 3);
    if(BMF->FileHeader.pSignature[0] != 'B' || BMF->FileHeader.pSignature[1] != 'M' || BMF->FileHeader.pSignature[2] != 'F')
    {
        LOG(Error, LogAssetLoader, "Attempted reading BMF file, but it had wrong header signature");
        delete[] buffer;
        delete BMF;
        return nullptr;
    }

    delete[] buffer;
    CHECKREAD(4);
    BMF->FileHeader.uMagic = (uint32_t)buffer[0] << 24 |
                             (uint32_t)buffer[1] << 16 |
                             (uint32_t)buffer[2] << 8  |
                             (uint32_t)buffer[3];

    if(BMF->FileHeader.uMagic != BLAME_MASTER_FILE_MAGIC)
    {
        LOG(Error, LogAssetLoader, "Attempted reading BMF file, but it had wrong header magic");
        delete[] buffer;
        delete BMF;
        return nullptr;
    }
    
    delete[] buffer;
    CHECKREAD(2);
    BMF->FileHeader.uVersion = (uint16_t)buffer[0] << 8  |
                               (uint16_t)buffer[1];

    if(BMF->FileHeader.uVersion > BLAME_MASTER_FILE_VERSION)
    {
        LOGF(Error, LogAssetLoader, "Attempted reading BMF file, but it's version is newer than current engine version supports: %u > %u", BMF->FileHeader.uVersion, BLAME_MASTER_FILE_VERSION);
        delete[] buffer;
        delete BMF;
        return nullptr;
    }

    // Fetch table
    delete[] buffer;
    CHECKREAD(4);
    BMF->FileTable.uObjectCount = ConvertChar::ToUInt32(buffer);

    // Allocate memory
    BMF->FileTable.pObjects = new BlameObjectTableEntry[BMF->FileTable.uObjectCount];
    BMF->Table.reserve(BMF->FileTable.uObjectCount);

    for(int i = 0; i < BMF->FileTable.uObjectCount; ++i)
    {
        // FIXME: Do we actually need 2 separate table entry containers?
        delete[] buffer;
        CHECKREAD(4);
        BMF->FileTable.pObjects[i].ID = ConvertChar::ToUInt32(buffer);

        delete[] buffer;
        CHECKREAD(4);
        BMF->FileTable.pObjects[i].offset = ConvertChar::ToUInt32(buffer);

        BMF->Table[BMF->FileTable.pObjects[i].ID] = BMF->FileTable.pObjects[i].offset;
    }
    
    delete[] buffer;
    return BMF;
#   undef CHECKREAD
}

void AssetLoader::CrawlContent(std::string Path)
{
    std::unique_ptr<IDirectory> directory = IPlatform::Get()->OpenLocalDirectory(Path);

    if(!directory->Exists()) {
        LOGF(Error, LogAssetLoader, "Attempted to crawl directory \"%s\", but it doesn't exist.", Path.c_str());
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
}

BlameMasterFileObjectContainer AssetLoader::ReadObject(BlameMasterFile* master, uint32_t offset)
{
#   define CHECKREAD(count) \
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

    delete[] buffer;
    CHECKREAD(1);
    container.object.uClassNameLength = ConvertChar::ToUInt8(buffer);
    if(container.object.uClassNameLength == 0)
    {
        LOG(Error, LogAssetLoader, "Attempted reading BMF file object, but it's class name length is 0");
        return {0};
    }

    delete[] buffer;
    CHECKREAD(container.object.uClassNameLength);
    memmove(container.object.pClassName, buffer, container.object.uClassNameLength);

    delete[] buffer;
    CHECKREAD(2);
    container.object.uNameLength = ConvertChar::ToUInt16(buffer);

    delete[] buffer;
    CHECKREAD(container.object.uNameLength);
    memmove(container.object.pName, buffer, container.object.uNameLength);

    delete[] buffer;
    CHECKREAD(4);
    container.object.uParent = ConvertChar::ToUInt32(buffer);

    delete[] buffer;
    CHECKREAD(2);
    container.object.uFieldsCount = ConvertChar::ToUInt16(buffer);

    container.object.pFields = new BlameMasterFileObjectField[container.object.uFieldsCount];
    for(int fieldIndex = 0; fieldIndex < container.object.uFieldsCount; ++fieldIndex)
    {
        delete[] buffer;
        CHECKREAD(1);
        container.object.pFields[fieldIndex].uType = ConvertChar::ToUInt8(buffer);

        delete[] buffer;
        CHECKREAD(1);
        container.object.pFields[fieldIndex].uFieldNameLength = ConvertChar::ToUInt8(buffer);

        delete[] buffer;
        CHECKREAD(container.object.pFields[fieldIndex].uFieldNameLength);
        memmove(container.object.pFields[fieldIndex].pFieldName, buffer, container.object.pFields[fieldIndex].uFieldNameLength);
        
        delete[] buffer;
        CHECKREAD(4);
        container.object.pFields[fieldIndex].uDataSize = ConvertChar::ToUInt32(buffer);

        delete[] buffer;
        CHECKREAD(container.object.pFields[fieldIndex].uDataSize);
        memmove(container.object.pFields[fieldIndex].pData, buffer, container.object.pFields[fieldIndex].uDataSize);
    }

    delete[] buffer;
    return container;
#   undef CHECKREAD
}

LoadedObjectDescriptor AssetLoader::LoadObject(uint32_t ObjectID)
{
    if(ObjectID == 0) {
        assert(!"Attempt at loading Nullptr Object with ID 0");
        return {};
    }

    for(BlameMasterFile* master : blameMasters)
    {
        if(master->bIsMounted)
        {
            if(master->Table.contains(ObjectID))
            {
                BlameMasterFileObjectContainer container = ReadObject(master, master->Table[ObjectID]);
                
                ObjectType* objType = ObjectTypeLibrary::Get().GetObjectType(container.object.pClassName);
                if(objType == nullptr) {
                    LOG(Error, LogAssetLoader, "Failed loading object, specified Class Name doesn't exist");
                    return {};
                }
                
                Object* object = objType->CreateInstance();

                object->Rename(container.object.pName);
                object->SetID(ObjectID);

                FieldContainer fields(container.object.uFieldsCount);

                for(int fieldID = 0; fieldID < container.object.uFieldsCount; ++fieldID) {
                    BlameMasterFileObjectField field = container.object.pFields[fieldID];
                    switch(field.uType) {
                        case (uint8_t)ObjectFieldType::Int: {
                            fields.SetInt(field.pFieldName, ConvertChar::ToInt32((char*)(field.pData)));
                            break;
                        }
                        case (uint8_t)ObjectFieldType::UInt: {
                            fields.SetUInt(field.pFieldName, ConvertChar::ToUInt32((char*)(field.pData)));
                            break;
                        }
                        case (uint8_t)ObjectFieldType::Float: {
                            fields.SetFloat(field.pFieldName, ConvertChar::ToFloat((char*)(field.pData)));
                            break;
                        }
                        case (uint8_t)ObjectFieldType::Double: {
                            // TODO: Double
                            fields.SetFloat(field.pFieldName, ConvertChar::ToFloat((char*)(field.pData)));
                            break;
                        }
                        case (uint8_t)ObjectFieldType::String: {
                            std::string str((char*)(field.pData), field.uDataSize);
                            fields.SetString(field.pFieldName, str);
                            break;
                        }
                        case (uint8_t)ObjectFieldType::Text: {
                            std::string str((char*)(field.pData), field.uDataSize);
                            fields.SetText(field.pFieldName, str);
                            break;
                        }
                        default: {
                            LOGF(Error, LogAssetLoader, "Unsupported field type: %u", field.uType);
                            break;
                        }
                    }
                }

                object->Serialize(fields);

                return {object, container.object.uParent};
            }
        }
    }

    return {nullptr, 0};
}

Resource* AssetLoader::LoadResource(std::string path)
{
    std::unique_ptr<IFile> file = IPlatform::Get()->OpenContentFile(path, FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);
    if(file->IsOpen())
    {
        LOGF(Log, LogAssetLoader, "Loaded Resource \"%s\".", path.c_str());
        return new Resource(file->FetchAllBinary());
    }

    // If requested resource is not found in local filesystem, search in archive files
    for(ArchiveFile* arch : resourceArchives)
    {
        if(arch->Table.contains(ArchiveUtility::Hash64(path))) {
            // TODO: Read resource
            break;
        }
    }

    LOGF(Error, LogAssetLoader, "Failed loading Resource: \"%s\". It doesn't exist or read permission is denied.", path.c_str());

    return nullptr;
}