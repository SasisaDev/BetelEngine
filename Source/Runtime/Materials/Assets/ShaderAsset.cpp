#include "ShaderAsset.h"

bool ShaderAssetType::bIsRegistered = AssetLibrary::RegisterAssetType<ShaderAssetType>();

void ShaderAsset::Deserialize(Artifact& data)
{
    if(!data.Has("VertexShaderPath") || !data.Has("VertexShaderPath")) {
        return;
    }
    VertexShaderPath = data.GetString("VertexShaderPath");
    FragmentShaderPath = data.GetString("FragmentShaderPath");
}