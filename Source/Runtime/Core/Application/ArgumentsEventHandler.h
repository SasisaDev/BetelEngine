#pragma once

#include "Arguments.h"

class ArgumentsEventHandler
{
public:
    std::vector<ArgumentEvent*> events;

    static ArgumentsEventHandler& Get() {
        static ArgumentsEventHandler handler;
        return handler;
    }

    template <typename EventType>
    static bool Register() {
        Get().events.push_back(new EventType);
        return true;
    }
};