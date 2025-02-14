#include "BetelImages.h"

bool BImGui::EditorImagesbInitialized = false;
std::vector<EditorTextureData> BImGui::EditorImages = std::vector<EditorTextureData>((size_t)BImGui::Img::MAX);

void BImGui::LoadEditorImages()
{
    if(EditorImagesbInitialized) {
        return;
    }

    EditorImages[static_cast<size_t>(Img::Browse32Icon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/Browse32.png");
    EditorImages[static_cast<size_t>(Img::Save32Icon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/Save32.png");
    EditorImages[static_cast<size_t>(Img::Visibility32Icon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/Visibility32.png");
    EditorImages[static_cast<size_t>(Img::Reload32Icon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/Reload32.png");
    EditorImages[static_cast<size_t>(Img::Cross32Icon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/Cross32.png");
    EditorImages[static_cast<size_t>(Img::None256Icon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/None256.png");

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

    for(EditorTextureData& data : EditorImages)
    {
        EditorImageLoader::Get().FreeTexture(data);
    }

    EditorImages.clear();
}