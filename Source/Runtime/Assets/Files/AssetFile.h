#pragma once
#include "../Data/SerialObject.h"

#include <Platform/Platform.h>

#include <string>
#include <map>

struct AssetHeader
{
    char signature[3] = {'B','A','F'};
    uint32_t magic = 0xABE7E7AF;
    uint16_t version = 0;
    uint16_t compressionMethod = 0; // 0 for none
};

/*
 * Container for serialized metadata about .asset file 
*/
class AssetFile 
{
protected:
    IFile* file;
    AssetHeader header;
    SerialObjectFieldTable ftable;
public:

};