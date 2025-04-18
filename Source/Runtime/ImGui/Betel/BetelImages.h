#pragma once
#include <vector>
#include <Debug/DebugImageLoader.h>

namespace BImGui
{
    enum class Img : size_t 
    {
        Browse32Icon,
        Save32Icon,
        Visibility32Icon,
        Reload32Icon,
        None256Icon,
        Cross32Icon,
        Pencil32Icon,
        Eraser32Icon,
        Plus32Icon,
        MAX
    };

    extern bool EditorImagesbInitialized;
    extern std::vector<DebugTextureData> EditorImages;

    // Call to initialize all images
    extern void LoadEditorImages();
    
    extern ImTextureID GetEdImage(BImGui::Img img);

    extern void UnloadEditorImages();
};