#include "ObjectEditViewsFactory.h"

#include "Texture/TextureEditView.h"
#include "Atlas/AtlasEditView.h"
#include <Engine/Engine.h>
#include <Editor/Editor.h>
#include <World/World.h>

std::unordered_map<std::string, std::function<std::unique_ptr<EditorToolkitWindow>(Object*)>> ObjectEditorViewsFactory::FactoryConstructors =
{
    {"TEX", [](Object* obj){return std::make_unique<TextureEditView>(obj);}},
    {"ATLAS", [](Object* obj){return std::make_unique<AtlasEditView>(obj);}},
    {"WORLD", [](Object* obj){Editor::Get()->TravelToWorld(dynamic_cast<World*>(obj)); return nullptr;}},
};