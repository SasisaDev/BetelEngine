#pragma once

#include <Platform/Platform.h>

class Asset;

class IAssetFactory
{
public:
    virtual Asset* Import(IFile* File){return nullptr;}
    virtual void Reimport(Asset* ExistingAsset, IFile* File){}
};