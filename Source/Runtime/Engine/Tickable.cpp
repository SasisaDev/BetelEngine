#include "Tickable.h"

void TickableManager::Tick(float DeltaTime)
{
    std::unordered_set<ITickable*>::iterator iterator;
    for(iterator = tickables.begin(); iterator != tickables.end(); iterator++)
    {
        (*iterator)->Tick(DeltaTime);
    }
}