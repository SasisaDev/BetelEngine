#include "../../AssetFactoryLibrary.h"

class Texture2DFactory : public IAssetFactory
{
    volatile static bool Registered;
public:
    virtual std::string GetName() override {return "Texture2DFactory";}
    virtual bool SuitableFor(IFile* File) override {return false;}
    virtual Asset* Import(IFile* File) override {return nullptr;}
    virtual Asset* CreateNew() override {return nullptr;}
    virtual void Reimport(Asset* ExistingAsset, IFile* File) override {}
};

volatile bool Texture2DFactory::Registered = AssetFactoryLibrary::Register<Texture2DFactory>();