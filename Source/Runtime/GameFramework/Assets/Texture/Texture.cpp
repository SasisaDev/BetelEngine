#include "Texture.h"

#include <Object/ObjectTypeLibrary.h>
#include <Resources/Resource.h>
#include <AssetLoader/AssetLoader.h>
#include <stb/stb_image.h>
#include <Engine/Engine.h>

bool ObjTextureType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<ObjTextureType>("TEX");

void ObjTexture::LoadTexture()
{
    // FIXME: Changing it to proper "texture.reset()" or avoiding "release", causes destruction of all Vulkan objects of the texture
    // This in turn causes violent Validation Layer panic, because resources are still in use by ImGui
    // If we use "texture.release", pointer gets freed and won't be destroyed ever, causing memory leak! Critical!
    /*if(texture.get() != nullptr) {
        texture.release();
    }*/

    if(path.empty()) {
        return;
    }

    Resource *Image = GEngine->GetAssetLoader()->LoadResource(path);

    int texWidth, texHeight, texChannels;
    unsigned char* pixels = stbi_load_from_memory(reinterpret_cast<stbi_uc*>(Image->GetBuffer().data()), Image->GetBuffer().size(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);

    sizeX = texWidth;
    sizeY = texHeight;

    texture = std::make_unique<ISamplerTexture>(texWidth, texHeight, pixels);

    stbi_image_free(pixels);
    delete Image;
}

void ObjTexture::Serialize(FieldContainer& cont)
{
    if(cont.IsSaving())
    {
        cont.SetString("path", path);
    } 
    else 
    {
        path = cont.GetString("path");
        LoadTexture();
    }
}