#include "Timer.h"

void TimerManager::SetTimer(TimerID& id, float tickDelay, Delegate<void> delegate, bool cycle, float cycleTime)
{
    TimerData timerData;
    timerData.startTime = CurrentTime;
    timerData.firstTickDelay = tickDelay;
    timerData.isCycle = cycle;
    timerData.tickTime = cycleTime;
    timerData.delegate = delegate;
}

void TimerManager::StopTimer(TimerID id)
{

}

void TimerManager::Tick(float deltaTime) 
{
    CurrentTime += deltaTime;

    for(int i = 0; i < Timers.size(); i++)
    {
        // All timers must make their first tick at specific delay. If delay is zero, we tick imediately
        if(Timers[i].firstTicked == false)
        {
            if((Timers[i].firstTickDelay + Timers[i].startTime) >= CurrentTime)
            {
                Timers[i].delegate.Execute();
                Timers[i].firstTicked = true;
                Timers[i].startTime = CurrentTime;
            }

            continue;
        }

        // Once we ticked for the first time, check if we should cycle
        if(Timers[i].isCycle) 
        {
            if(Timers[i].tickTime + Timers[i].startTime >= CurrentTime) 
            {
                Timers[i].startTime = CurrentTime;
                Timers[i].delegate.Execute();
            }

            continue;
        }

        // If we don't need to cycle, remove the timer
        Timers.erase(Timers.begin() + i);
        // Since we just removed Timers[i], our next element Timers[i+1] becomes new Timers[i]
        // Because of this we decrement i to avoid skip
        --i;
    }
}