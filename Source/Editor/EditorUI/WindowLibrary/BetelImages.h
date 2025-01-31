#pragma once
#include <vector>
#include <EditorUI/EditorImageLoader.h>

namespace BImGui
{
    enum class Img : size_t 
    {
        Browse32Icon,
        Save32Icon,
        Visibility32Icon,
        Reload32Icon,
        MAX
    };

    extern std::vector<EditorTextureData> Images;

    // Call to initialize all images
    extern void LoadEditorImages();
    
    extern ImTextureID GetEdImage(BImGui::Img img);
};