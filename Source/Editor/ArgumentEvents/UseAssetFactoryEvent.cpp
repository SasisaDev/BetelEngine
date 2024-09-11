#include <Core/Application/ArgumentsEventHandler.h>

class UseAssetFactoryArgumentEvent : public ArgumentEvent
{
    static bool isRegistered;
public:
    virtual bool Handle(ArgumentCollection& collection) override
    {
        if(collection.Has("UseFactory"))
        {
            return false;
        }
        return true;
    }
};

bool UseAssetFactoryArgumentEvent::isRegistered = ArgumentsEventHandler::Register<UseAssetFactoryArgumentEvent>();