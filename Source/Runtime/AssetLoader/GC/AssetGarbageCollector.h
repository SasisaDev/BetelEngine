#pragma once

#include <AssetLoader/AssetLoader.h>

class ObjectLibrary;

class AssetGarbageCollector
{
protected:
    ObjectLibrary* library;
public:
    AssetGarbageCollector(ObjectLibrary* lib) : library(lib) {}
};