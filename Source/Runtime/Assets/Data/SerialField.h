#pragma once

#include <optional>

enum class ESerialObjectFieldType : uint8_t
{
    Int,
    Byte,
    Array,
    Map,
    String,
    // ByteStream is a raw byte data, that will be compressed
    ByteStream,
    // Encapsulates a whole Serial Object
    Object,
};

enum ESerialObjectFieldFlags
{
    SERIAL_OBJECT_FIELD_LAZY = (0 << 0),
    SERIAL_OBJECT_FIELD_COMPRESSED = (0 << 1),
};

/*
 * Serialized member field of an Object derived class 
 * Contains metadata about it's data
 * Used as an entry for SerialObjectFieldTable
*/
struct SerialObjectFieldMetadata
{
    uint16_t fieldNameSize;
    char* fieldName;
    uint8_t type;
    uint8_t flags;
    uint32_t offset;
    uint32_t size;
    uint32_t originalSize;
};

/*
 * Metadata container for fields
 * Goes right after the file header, if exists
 * First data block if file doesn't have a header.
*/
struct SerialObjectFieldTable 
{
    uint16_t fieldCount;
    SerialObjectFieldMetadata* fields;
};