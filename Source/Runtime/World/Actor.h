#pragma once

#include <set>

class Actor
{
protected:
    Actor* Parent;
    std::set<Actor*> Children;
public:

};