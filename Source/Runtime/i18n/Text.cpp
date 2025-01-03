#include "Text.h"
#include <Engine/Engine.h>

std::string Text::NOENGINE = std::string("NOENGINE");

std::string& Text::Get() 
{
    if(TranslationMemoize.has_value()) {
        return TranslationMemoize.value();
    }

    if(!GEngine) {
        LOG(Error, LogI18N, "Text fetch can't be performed before Engine class is initialized!");
        return NOENGINE;
    }

    return TranslationMemoize.emplace(GEngine->GetTextManager().Fetch(Domain, Subdomain, Name));
}