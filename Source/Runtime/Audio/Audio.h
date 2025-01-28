#pragma once

#include <Object/Object.h>
#include <memory>
#include "AudioSample.h"

class ObjAudio : public Object
{
    std::shared_ptr<AudioSample> audio;
public:

    // TODO: Loading audio on serialization load event

};

class ObjAudioType : public ObjectType
{
    static bool bRegistered;
public:
    virtual Object* CreateInstance() override { return new ObjAudio; }
    virtual std::string_view DisplayName() override {return "Audio";}
};