#include "BetelImages.h"

bool BImGui::EditorImagesbInitialized = false;
std::vector<DebugTextureData> BImGui::EditorImages = std::vector<DebugTextureData>((size_t)BImGui::Img::MAX);

void BImGui::LoadEditorImages()
{
    if(EditorImagesbInitialized) {
        return;
    }

    EditorImages[static_cast<size_t>(Img::Browse32Icon)] = DebugImageLoader::StaticLoadTextureFromFile("./Editor/Browse32.png");
    EditorImages[static_cast<size_t>(Img::Save32Icon)] = DebugImageLoader::StaticLoadTextureFromFile("./Editor/Save32.png");
    EditorImages[static_cast<size_t>(Img::Visibility32Icon)] = DebugImageLoader::StaticLoadTextureFromFile("./Editor/Visibility32.png");
    EditorImages[static_cast<size_t>(Img::Reload32Icon)] = DebugImageLoader::StaticLoadTextureFromFile("./Editor/Reload32.png");
    EditorImages[static_cast<size_t>(Img::Cross32Icon)] = DebugImageLoader::StaticLoadTextureFromFile("./Editor/Cross32.png");
    EditorImages[static_cast<size_t>(Img::None256Icon)] = DebugImageLoader::StaticLoadTextureFromFile("./Editor/None256.png");
    EditorImages[static_cast<size_t>(Img::Pencil32Icon)] = DebugImageLoader::StaticLoadTextureFromFile("./Editor/Pencil32.png");
    EditorImages[static_cast<size_t>(Img::Eraser32Icon)] = DebugImageLoader::StaticLoadTextureFromFile("./Editor/Eraser32.png");
    EditorImages[static_cast<size_t>(Img::Plus32Icon)] = DebugImageLoader::StaticLoadTextureFromFile("./Editor/Plus32.png");

    EditorImagesbInitialized = true;
}

ImTextureID BImGui::GetEdImage(BImGui::Img img) 
{
    return (ImTextureID)EditorImages[static_cast<size_t>(img)].DS;
}

void BImGui::UnloadEditorImages()
{
    if(!EditorImagesbInitialized) {
        return;
    }

    for(DebugTextureData& data : EditorImages)
    {
        DebugImageLoader::Get().FreeTexture(data);
    }

    EditorImages.clear();
}