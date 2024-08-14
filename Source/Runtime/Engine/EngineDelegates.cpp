#include "EngineDelegates.h"

TMulticastDelegate<World*> EngineDelegates::OnWorldLoad;
TMulticastDelegate<World*> EngineDelegates::OnWorldUnload;