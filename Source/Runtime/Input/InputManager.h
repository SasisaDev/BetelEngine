#pragma once

#include <Delegate/Delegate.h>
#include <Core/Window/WindowManager.h>
#include <unordered_map>

#include "InputEvent.h"

/*
 * InGame Input Event Manager
 * Only handles game events, everything aside them can be hooked by Window event handler. 
 */
class InputManager
{
    void InternalHandleEvent(SDL_Event e);
protected:
    bool IsActive = true;
    bool IsGamepad = false;
public:
    InputManager() {
        WindowManager::OnSDLEvent.BindMember(this, &InputManager::InternalHandleEvent);
    }

    /*
    * Multicast delegate, bound functions will be called
    * each time the input event occurs.
    */
    MulticastDelegate<InputEvent&> OnInputEvent;
    
};