#pragma once

#include <Delegate/Delegate.h>

class World;

class EngineDelegates
{
public:
    static TMulticastDelegate<World*> OnWorldLoad;
    static TMulticastDelegate<World*> OnWorldUnload;
};