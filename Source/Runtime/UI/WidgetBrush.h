#pragma once

#include <Materials/Assets/MaterialAsset.h>
#include <GameFramework/Assets/Texture2D/Texture2D.h>
#include <Assets/AssetSmartPointers.h>
#include <Math/Color.h>

/*
 * Set of data for graphical elements
 */
struct WidgetBrush {
    AssetPtr<ShaderAsset> shader;
    AssetPtr<Texture2DAsset> texture;
    Color color;

    WidgetBrush()
    : shader("Editor/Content/Default/Widgets/WidgetShader"),
      texture("Editor/Content/Default/Widgets/WidgetDefaultTexture"),
      color(1, 1, 1, 1) 
    {}
};