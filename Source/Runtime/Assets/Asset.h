#pragma once

#include "Artifact/Artifact.h"
#include <string>

class Asset
{
    std::string path;
public:

    virtual Artifact Serialize() {return {};}
    virtual void Deserialize(const Artifact& artifact) {}
};

class AssetType
{
    static bool bIsRegistered;
public:
    std::string TypeName;

    virtual Asset* CreateInstance(){return new Asset;}
};