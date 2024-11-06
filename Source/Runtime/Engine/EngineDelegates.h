#pragma once

#include <Delegate/Delegate.h>

class World;
class Asset;
class AssetLibrary;
struct InputEvent;

class EngineDelegates
{
public:
    static MulticastDelegate<World*> OnWorldLoad;
    static MulticastDelegate<World*> OnWorldUnload;

    static MulticastDelegate<Asset*> OnAssetLoaded;
    static MulticastDelegate<AssetLibrary*> OnAssetCrawlingFinished;

    static MulticastDelegate<InputEvent*> OnInputRaw;
};