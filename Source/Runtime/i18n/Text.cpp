#include "Text.h"
#include <Engine/Engine.h>

#include <cassert>

const std::string& Text::Get() 
{
    if(TranslationMemoize.has_value()) {
        return TranslationMemoize.value();
    }
    
    assert(GEngine != nullptr && "Text fetch can't be performed before Engine class is initialized!");

    return TranslationMemoize.emplace(GEngine->GetTextManager()->Fetch(ID));
}