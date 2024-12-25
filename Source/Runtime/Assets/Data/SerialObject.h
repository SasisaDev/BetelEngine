#pragma once

#include "SerialField.h"

#include <vector>
#include <string>

struct SerialObject 
{
    SerialObjectFieldTable ftable;
};

struct SerialObjectMetadata
{
    uint32_t objectID;
    uint16_t objectNameSize;
    char* objectName;
    uint8_t type;
    uint32_t offset;
    uint32_t size;
};

/*
 * Serialized representation of an Object derived class 
 * Contains metadata about it's fields
*/
struct SerialObjectTable 
{
    uint16_t objectCount;
    SerialObjectMetadata* objects;
};