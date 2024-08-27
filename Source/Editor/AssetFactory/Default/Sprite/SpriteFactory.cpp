#include "../../AssetFactoryLibrary.h"

class SpriteFactory : public IAssetFactory
{
    volatile static bool Registered;
public:

};

volatile bool SpriteFactory::Registered = AssetFactoryLibrary::Register<SpriteFactory>();