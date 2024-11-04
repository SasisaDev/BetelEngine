#include <Core/Application/ArgumentsEventHandler.h>

#include <AssetFactory/AssetFactoryLibrary.h>
#include <Log/Logger.h>
#include <Assets/Asset.h>
#include <Assets/AssetFile/AssetFile.h>

#include <cassert>

class UseAssetFactoryArgumentEvent : public ArgumentEvent
{
    static bool isRegistered;
public:
    virtual bool Handle(ArgumentCollection& collection) override
    {

        if(collection.Has("UseFactory"))
        {
            if(!(collection.Has("Input") && collection.Has("Output"))) {
                LOG(Error, LogFactoryCommand, "No Input and Output files specified.");
                return false;
            }

            std::string FactoryName = collection.Get("UseFactory");
            IPath input = collection.Get("Input");
            IPath output = collection.Get("Output");

            IAssetFactory* factory = nullptr;

            for(IAssetFactory* object : AssetFactoryLibrary::Get()->GetFactories()) {
                if(object->GetName() == FactoryName) {
                    factory = object;
                    break;
                }
            }

            if(!factory) {
                assert(!"Specified factory doesn't exist.");
                return false;
            }

            IFile* inputFile = IPlatform::Get()->OpenFile(input, FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY);
            IFile* outputFile = IPlatform::Get()->OpenFile(output, FILE_ACCESS_FLAG_WRITE | FILE_ACCESS_FLAG_BINARY);

            if(!factory->SuitableFor(inputFile)) {
                assert(!"Specified factory doesn't support this file type.");
                return false;
            }

            Asset* asset = factory->Import(inputFile);

            Artifact outArtifact = asset->Serialize();

            AssetFile outAssetFile;
            outAssetFile << outArtifact;

            outAssetFile.WriteToDevice(outputFile);
            
            return false;
        }
        return true;
    }
};

bool UseAssetFactoryArgumentEvent::isRegistered = ArgumentsEventHandler::Register<UseAssetFactoryArgumentEvent>();