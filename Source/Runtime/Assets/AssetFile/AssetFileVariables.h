#pragma once

#include <cstdint>

enum class AssetFileVariableType : uint8_t
{
    Integer,
    UInteger,
    Float,
    Double,
    String,
    LocalizedText,
    Structure,
    Custom
};

enum class AssetFileVariableModifier : uint8_t
{
    Single,
    Array,
    Map
};

struct AssetFileVariableSingle
{
    AssetFileVariableType type;
    AssetFileVariableModifier modifier = AssetFileVariableModifier::Single;

    uint8_t nameSize;
    unsigned char* pName;

    uint32_t dataSize;
    unsigned char* pData;
};

// Array

struct AssetFileVariableArrayElement
{
    uint32_t dataSize;
    unsigned char* pData;
};

struct AssetFileVariableArray
{
    AssetFileVariableType type;
    AssetFileVariableModifier modifier = AssetFileVariableModifier::Array;

    uint8_t nameSize;
    unsigned char* pName;

    uint16_t entryCount;
    AssetFileVariableArrayElement* pEntries;
};

// Map

struct AssetFileVariableMapPair
{
    uint32_t keySize;
    AssetFileVariableSingle* pKey;
    uint32_t valueSize;
    AssetFileVariableSingle* pValue;
};

struct AssetFileVariableArray
{
    AssetFileVariableType keyType;
    AssetFileVariableType valueType;
    AssetFileVariableModifier modifier = AssetFileVariableModifier::Array;

    uint8_t nameSize;
    unsigned char* pName;

    uint16_t entryCount;
    AssetFileVariableMapPair* pEntries;
};