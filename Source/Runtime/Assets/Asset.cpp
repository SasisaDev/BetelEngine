#include "Asset.h"
#include "AssetLibrary.h"

bool AssetType::bIsRegistered = AssetLibrary::RegisterAssetType<AssetType>(); 