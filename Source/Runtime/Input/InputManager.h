#pragma once

#include <Delegate/Delegate.h>
#include <Core/Window/Window.h>

class InputManager
{
public:
    /*
    * Multicast delegate, bound functions will be called each time
    * an input event has occured.
    *
    * Arguments:
    *   window_t    WindowID
    *   uint8_t     EventType
    *   uint32_t    EventKey
    *   void*       EventData
    */
    MulticastDelegate<window_t, uint8_t, uint32_t, void*> OnInputEvent;
    
};