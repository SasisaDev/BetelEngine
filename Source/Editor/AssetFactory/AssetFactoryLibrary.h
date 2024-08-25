#pragma once

#include <vector>

#include "AssetFactory.h"

class AssetFactoryLibrary
{
    std::vector<IAssetFactory*> Factories;
public:
    static AssetFactoryLibrary* Get();

    std::vector<IAssetFactory*> GetFactories() const {return Factories;}

    template <typename FactoryType>
    static bool Register() {
        Get()->Factories.push_back(new FactoryType);
        return true;
    }
};