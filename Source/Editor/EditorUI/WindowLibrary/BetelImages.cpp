#include "BetelImages.h"

std::vector<EditorTextureData> BImGui::Images = std::vector<EditorTextureData>((size_t)BImGui::Img::MAX);

void BImGui::LoadEditorImages()
{
    Images[static_cast<size_t>(Img::Browse32Icon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/Browse32.png");
    Images[static_cast<size_t>(Img::Save32Icon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/Save32.png");
    Images[static_cast<size_t>(Img::Visibility32Icon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/Visibility32.png");
    Images[static_cast<size_t>(Img::Reload32Icon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/Reload32.png");
}

ImTextureID BImGui::GetEdImage(BImGui::Img img) {
    return (ImTextureID)Images[static_cast<size_t>(img)].DS;
}