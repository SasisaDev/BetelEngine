#include "EditorDelegates.h"

MulticastDelegate<EditorTool*> EditorDelegates::ToolCreated = MulticastDelegate<EditorTool*>();
MulticastDelegate<EditorTool*> EditorDelegates::ToolBeginDestroy = MulticastDelegate<EditorTool*>();