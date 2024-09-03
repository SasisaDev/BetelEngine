#pragma once

#include "Artifact/Artifact.h"

class Asset
{
    
public:

    virtual Artifact Serialize() {return {};}
    virtual void Deserialize(const Artifact& artifact) {}
};