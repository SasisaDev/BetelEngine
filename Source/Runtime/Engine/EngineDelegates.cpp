#include "EngineDelegates.h"

MulticastDelegate<World*> EngineDelegates::OnWorldLoad;
MulticastDelegate<World*> EngineDelegates::OnWorldUnload;