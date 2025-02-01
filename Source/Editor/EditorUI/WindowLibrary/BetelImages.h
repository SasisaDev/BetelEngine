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
        None256Icon,
        MAX
    };

    extern bool EditorImagesbInitialized;
    extern std::vector<EditorTextureData> EditorImages;

    // Call to initialize all images
    extern void LoadEditorImages();
    
    extern ImTextureID GetEdImage(BImGui::Img img);

    extern void UnloadEditorImages();
};