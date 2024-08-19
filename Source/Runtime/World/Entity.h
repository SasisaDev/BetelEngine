#pragma once

#include <set>

class Entity
{
protected:
    Entity* Parent;
    std::set<Entity*> Children;
public:

};