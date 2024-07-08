#pragma once

#include "Timer.h"
#include "Tickable.h"

class Engine
{
protected:
    TickableManager tickManager;
    TimerManager timerManager;
public:
    TickableManager& GetTickManager() {return tickManager;}
    TimerManager& GetTimer() {return timerManager;}

    void Tick(float DeltaTime);
};

extern Engine* GEngine;