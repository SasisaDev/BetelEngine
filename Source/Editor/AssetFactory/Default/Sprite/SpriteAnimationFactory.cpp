#include "../../AssetFactoryLibrary.h"

class SpriteAnimationFactory : public IAssetFactory
{
    volatile static bool Registered;
public:

};

volatile bool SpriteAnimationFactory::Registered = AssetFactoryLibrary::Register<SpriteAnimationFactory>();