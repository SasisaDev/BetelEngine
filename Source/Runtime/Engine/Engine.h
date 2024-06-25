#pragma once

#include "Timer.h"

class Engine
{
protected:
    TimerManager timerManager;
public:
    TimerManager& GetTimer() {return timerManager;}

    void Tick(float DeltaTime);
};

extern Engine* GEngine;