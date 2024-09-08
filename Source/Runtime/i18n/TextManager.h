#pragma once

#include "Asset/LocaleTableAsset.h"
#include <Assets/AssetSmartPointers.h>
#include <vector>

class TextManager
{
protected:
    std::vector<AssetPtr<LocaleTableAsset>> locales;

public:

};