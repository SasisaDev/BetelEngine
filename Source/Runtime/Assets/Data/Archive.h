#pragma once

#include "SerialObject.h"
#include <vector>
#include <map>

class Archive 
{
    std::map<SerialField, void*> data;
    bool bIsLoading = false;
public:
    inline bool IsLoading() const {return bIsLoading;}

};