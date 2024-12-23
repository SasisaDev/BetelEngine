#pragma once
#include "../Data/SerialObject.h"

#include <Platform/Platform.h>

#include <string>
#include <map>

struct BundleHeader
{
    char signature[3] = {'B','D','L'};
    uint32_t magic = 0xABD7F17E;
    uint16_t version = 0;
    uint16_t compressionMethod = 0; // 0 for none
};

/*
 * Container for serialized metadata about .bundle file 
*/
class BundleFile 
{
protected:
    IFile* file;
    BundleHeader header;
    SerialObjectTable objtable;
    bool bIsMount = false;
public:
    
};