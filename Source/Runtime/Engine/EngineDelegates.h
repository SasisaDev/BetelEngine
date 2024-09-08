#pragma once

#include <Delegate/Delegate.h>

class World;

class EngineDelegates
{
public:
    static MulticastDelegate<World*> OnWorldLoad;
    static MulticastDelegate<World*> OnWorldUnload;
};