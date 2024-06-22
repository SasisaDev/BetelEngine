#pragma once

#include <Delegate/Delegate.h>

class InputManager
{
protected:
    /*
    * Multicast delegate, bound functions will be called each time
    * an input event has occured.
    *
    * Arguments:
    *   uint8_t     WindowID
    *   uint8_t     EventType
    *   uint32_t    EventKey
    *   void*       EventData
    */
    TMulticastDelegate<uint8_t, uint8_t, uint32_t, void*> OnInputEvent;
    
};