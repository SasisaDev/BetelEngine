#pragma once

#include <Delegate/Delegate.h>

class World;
class Asset;
class AssetLibrary;
class BundleFile;
class AssetFile;
struct InputEvent;

class EngineDelegates
{
public:
    static MulticastDelegate<World*> OnWorldLoad;
    static MulticastDelegate<World*> OnWorldUnload;

    static MulticastDelegate<BundleFile*> OnBundleFileMounted;
    static MulticastDelegate<AssetFile*> OnAssetFileMounted;
    static MulticastDelegate<Asset*> OnAssetLoaded;
    static MulticastDelegate<AssetLibrary*> OnAssetCrawlingFinished;

    static MulticastDelegate<InputEvent*> OnInputRaw;
};