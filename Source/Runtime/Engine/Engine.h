#pragma once

#include "Timer.h"
#include "Tickable.h"
#include <World/World.h>

class Engine
{
protected:
    TickableManager tickManager;
    TimerManager timerManager;
    World* world;
public:
    TickableManager& GetTickManager() {return tickManager;}
    TimerManager& GetTimer() {return timerManager;}

#pragma region "World API"
    inline World* GetWorld() const {return world;}
#pragma endregion "World API"

    void Tick(float DeltaTime);
};

extern Engine* GEngine;