#pragma once

#include <vector>
#include <Delegate/Delegate.h>

typedef unsigned int TimerID;

struct TimerData
{
    TimerID id;
    Delegate<void> delegate;
    float startTime = 0;
    float firstTickDelay = 0;
    bool firstTicked = false;
    float tickTime = 0;
    bool isCycle = false;
};

class TimerManager {
protected:
    std::vector<TimerData> Timers;
    float CurrentTime = 0;
public:
    void SetTimer(TimerID& id, float tickDelay, Delegate<void> delegate, bool cycle = false, float cycleTime = 0);
    void StopTimer(TimerID id);

    void Tick(float deltaTime);
};