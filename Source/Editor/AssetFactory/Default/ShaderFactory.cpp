#include "../AssetFactoryLibrary.h"

class ShaderFactory : public IAssetFactory
{
    volatile static bool Registered;
public:

};

volatile bool ShaderFactory::Registered = AssetFactoryLibrary::Register<ShaderFactory>();