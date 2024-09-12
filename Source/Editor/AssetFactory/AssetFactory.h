#pragma once

#include <Platform/Platform.h>

class Asset;

class IAssetFactory
{
public:
    virtual std::string GetName() {return "AssetFactory";}
    virtual bool SuitableFor(IFile* File) {return false;}
    virtual Asset* Import(IFile* File){return nullptr;}
    virtual void Reimport(Asset* ExistingAsset, IFile* File){}
};