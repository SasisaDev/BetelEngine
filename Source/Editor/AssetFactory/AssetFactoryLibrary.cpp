#include "AssetFactoryLibrary.h"

 AssetFactoryLibrary* AssetFactoryLibrary::Get()
 {
    static AssetFactoryLibrary Library;
    return &Library;
 }