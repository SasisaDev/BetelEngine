#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <GameFramework/Assets/Texture/Texture.h>

#include <EditorUI/WindowLibrary/BetelInputs.h>

#include <imgui/backends/imgui_impl_vulkan.h>

class TextureEditView : public EditorToolkitWindow
{
    ObjTexture* texture = nullptr;
    VkDescriptorSet DS;
public:
    TextureEditView(ObjTexture* tex) : texture(tex) 
    {
        DS = ImGui_ImplVulkan_AddTexture(texture->GetTexture()->GetSampler(), texture->GetTexture()->GetImageView(), VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }

    ~TextureEditView() 
    {
        ImGui_ImplVulkan_RemoveTexture(DS);
    }

    virtual void OnGUI(Window* window) override
    {
        assert(texture != nullptr && "Texture Edit View must be provided with a Texture Object to modify!");    

        BImGui::InputString("Name", const_cast<std::string&>(texture->GetName()));
        ImGui::Image((ImTextureID)DS, ImVec2(250, 250));
    }
};