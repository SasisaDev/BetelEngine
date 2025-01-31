#include "ObjectEditViewsFactory.h"

#include "Texture/TextureEditView.h"

std::unordered_map<std::string, std::function<std::unique_ptr<EditorToolkitWindow>(Object*)>> ObjectEditorViewsFactory::FactoryConstructors =
{
    {"TEX", [](Object* obj){return std::make_unique<TextureEditView>(obj);}},
};