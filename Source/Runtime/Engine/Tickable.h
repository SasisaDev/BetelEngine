#pragma once

#include <unordered_set>

struct ITickable
{
    ITickable();

    virtual void Tick(float DeltaTime){}
};

class TickableManager
{
protected:
    std::unordered_set<ITickable*> tickables;

public:

    void Tick(float DeltaTime);
};