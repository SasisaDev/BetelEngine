#include "BetelImages.h"

std::vector<EditorTextureData> BImGui::Images = std::vector<EditorTextureData>((size_t)BImGui::Img::MAX);

void BImGui::LoadEditorImages()
{
    Images[static_cast<size_t>(Img::BrowseIcon)] = EditorImageLoader::StaticLoadTextureFromFile("./Editor/Browse32.png", 32, 32);
}

ImTextureID BImGui::GetEdImage(BImGui::Img img) {
    return (ImTextureID)Images[static_cast<size_t>(img)].DS;
}