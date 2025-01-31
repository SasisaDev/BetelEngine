#pragma once
#include <vector>
#include <EditorUI/EditorImageLoader.h>

namespace BImGui
{
    enum class Img : size_t 
    {
        BrowseIcon,
        MAX
    };

    extern std::vector<EditorTextureData> Images;

    extern void LoadEditorImages();
    extern ImTextureID GetEdImage(BImGui::Img img);
};