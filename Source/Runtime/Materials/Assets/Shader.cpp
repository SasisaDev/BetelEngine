#include "Shader.h"
#include <Platform/Platform.h>
#include <Core/Application/Application.h>
#include <Object/ObjectTypeLibrary.h>
#include <UI/RenderLayer/WidgetRenderLayer.h>


bool ObjShaderType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<ObjShaderType>("SHADER");

/*void ObjShader::Deserialize(Artifact& data)
{
    VertexShaderPath = data.GetStringOrDefault("VertexShaderPath");
    FragmentShaderPath = data.GetStringOrDefault("FragmentShaderPath");

    BlendEnable = data.GetBoolOrDefault("BlendEnable", true);
    SampleShadingEnable = data.GetBoolOrDefault("SampleShadingEnable", false);

    if(VertexShaderPath.size() > 1 && FragmentShaderPath.size() > 1) {
        std::unique_ptr<IFile> VertFile = IPlatform::Get()->OpenLocalFile(VertexShaderPath, FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);
        std::unique_ptr<IFile> FragFile = IPlatform::Get()->OpenLocalFile(FragmentShaderPath, FILE_ACCESS_FLAG_READ | FILE_ACCESS_FLAG_BINARY | FILE_ACCESS_FLAG_ATE);

        if(!VertFile->IsOpen() || !FragFile->IsOpen()) {
            LOGF(Error, LogShaderAsset, "Shaders not found: \"%s\" or \"%s\"!", VertexShaderPath.c_str(), FragmentShaderPath.c_str());
        }

        ShaderDescriptorLayout descriptorsLayout;

        pShader = std::make_shared<IShader>(GApplication->GetRender()->GetLayerTyped<UIRenderLayer>()->GetRenderPass(), VertFile->FetchAllBinary(), FragFile->FetchAllBinary(), descriptorsLayout);

        if(!pShader->IsValid())
        {
            LOG(Error, LogWidget, "Widget brush shader creation failed");
            LOGF(Error, LogWidget, "\tVertex Shader: \"%s\"", VertexShaderPath.c_str());
            LOGF(Error, LogWidget, "\tFragment Shader: \"%s\"", FragmentShaderPath.c_str());
        }
    }
}*/