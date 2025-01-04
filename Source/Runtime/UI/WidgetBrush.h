#pragma once

#include <Materials/Assets/MaterialAsset.h>
#include <GameFramework/Assets/Texture/Texture.h>
#include <Assets/AssetSmartPointers.h>
#include <Math/Color.h>

/*
 * Set of data for graphical elements
 */
struct WidgetBrush 
{
    AssetPtr<ShaderAsset> shader;
    AssetPtr<ObjTexture> texture;
    Color color;

    WidgetBrush()
    {
      static AssetPtr<ShaderAsset> DefaultBrushShader = AssetPtr<ShaderAsset>("Content/Default/Widgets/WidgetShader");
      static AssetPtr<ObjTexture> DefaultBrushTexture = AssetPtr<ObjTexture>("Content/Default/Widgets/WidgetDefaultTexture");
      static Color DefaultBrushColor = Color(1,1,1,1);

      shader = DefaultBrushShader;
      texture = DefaultBrushTexture;
      color = DefaultBrushColor;
    }
};