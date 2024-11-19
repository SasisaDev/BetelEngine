#pragma once

#include <Delegate/Delegate.h>

class EditorTool;

class EditorDelegates
{
public:
    static MulticastDelegate<EditorTool*> ToolCreated;
    static MulticastDelegate<EditorTool*> ToolBeginDestroy;
};