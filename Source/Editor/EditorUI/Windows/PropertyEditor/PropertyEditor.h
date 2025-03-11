#pragma once

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>
#include <Editor/Editor.h>
#include <Reflection/IPropertyProvider.h>

#include <ImGui/Betel/BetelInputs.h>

class EditorPropertyEditor : public EditorToolkitWindow
{
public:
    EditorPropertyEditor() {}

    virtual const char *GetName() override { return "Details"; }

    void DrawString(Property& prop)
    {
        std::string& string = *static_cast<std::string*>(prop.value);
        std::string name = "##";
        name += prop.name;
        BImGui::InputString(name.c_str(), string);
    }

    void DrawObject(Property& prop)
    {
        PropertyData_Object callbacks = *static_cast<PropertyData_Object*>(prop.value);

        uint32_t objectID = callbacks.GetID.Execute();
        std::string filterString;

        std::string name = "##";
        name += prop.name;
        if(BImGui::InputObject(name.c_str(), objectID, filterString, callbacks.TypeFilter.c_str()))
        {
            callbacks.Reset.Execute(objectID);
        }
    }

    virtual void OnGUI(Window *window) override
    {
        ImGui::SetNextWindowSize(ImVec2(350, 200), ImGuiCond_Once);
        ImGui::SetNextWindowBgAlpha(1);
        if (ImGui::Begin(GetName(), 0, ImGuiWindowFlags_NoCollapse))
        {
            static ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV;

            if (ImGui::BeginTable("table1", 2, flags))
            {
                //ImGui::TableSetupColumn("Name");
                //ImGui::TableSetupColumn("Value");
                //ImGui::TableHeadersRow();

                PropertyContainer properties = {};
                if(IPropertyProvider *provider = dynamic_cast<IPropertyProvider*>(Editor::Get()->GetSelectedObject()))
                {
                    properties = provider->GetEditorReflectedProperties();
                }

                if(properties.properties.size() == 0) {
                    ImGui::TextUnformatted("No properties available");

                    ImGui::EndTable();
                    ImGui::End();
                    return;
                }

                for (int row = 0; row < properties.properties.size(); row++)
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextUnformatted(properties.properties[row].name.data());
                    ImGui::TableSetColumnIndex(1);
                    switch(properties.properties[row].type)
                    {
                        case PropertyType::String: 
                        {
                            DrawString(properties.properties[row]);
                            break;
                        }
                        case PropertyType::Object: 
                        {
                            DrawObject(properties.properties[row]);
                            break;
                        }
                        default:
                        {
                            ImGui::TextUnformatted("Unsupported Type");
                            break;
                        }
                    };
                }

                properties.Cleanup();
                ImGui::EndTable();
            }
        }
        ImGui::End();
    }
};