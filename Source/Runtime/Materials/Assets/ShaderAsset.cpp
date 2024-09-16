#include "ShaderAsset.h"

bool ShaderAssetType::bIsRegistered = AssetLibrary::RegisterAssetType<ShaderAssetType>();

void ShaderAsset::Deserialize(Artifact& data)
{
    VertexShaderPath = data.GetStringOrDefault("VertexShaderPath");
    FragmentShaderPath = data.GetStringOrDefault("FragmentShaderPath");

    BlendEnable = data.GetBoolOrDefault("BlendEnable", true);
    SampleShadingEnable = data.GetBoolOrDefault("SampleShadingEnable", false);
}