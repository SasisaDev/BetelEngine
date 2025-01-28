#pragma once

#include <set>

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>
#include <Object/ObjectTypeLibrary.h>

#include <imgui/imgui_internal.h>
#include <EditorUI/WindowLibrary/BetelInputs.h>

class EditorObjectExplorer : public EditorToolkitWindow
{
    struct HierarchyNode
    {
        std::string Name;
        std::string ID;
        std::string Parent;
        bool Selected = false;
        bool Open = false;
        std::vector<HierarchyNode> Children;
    };

protected:
    Text TabName = Text("EditorUI", "ObjectExplorer", "TabName");
    std::string TranslatedName;

    const char *HierarchyName = "##ObjectsHierarchy";
    const char *ContentsName = "##ObjectsContents";
    const char *FilterName = "##ObjectsFilter";
    std::string Filter;

    std::vector<HierarchyNode> hierarchy;
    HierarchyNode *currentSelection = nullptr;

    ImGuiTreeNodeFlags baseTreeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

public:
    EditorObjectExplorer()
    {
        TranslatedName = TabName.Get();
        UpdateHierarchy();
    }

    virtual const char* GetName() override {return TranslatedName.c_str();}

    void UpdateHierarchy()
    {
        ObjectTypeLibrary &lib = ObjectTypeLibrary::Get();
        auto map = lib.GetObjectTypes();

        // Clear outdated hierarchy
        hierarchy.clear();

        // Search through the whole types map to build hierarchy
        for(auto it = map.begin(); it != map.end(); it++) {
            if(!it->second->ShowInEditor()) {
                continue;
            }

            HierarchyNode node;
            node.ID = it->first;
            node.Name = it->second->DisplayName();
            
            // Add to the global hierarchy if has no parent
            // FIXME: Or to parent if it has a parent
            if(it->second->GetParent().empty()) {
                hierarchy.emplace_back(node);
            } else {
                for(HierarchyNode& parent : hierarchy) {
                    if(parent.ID == it->second->GetParent()) {
                        parent.Children.emplace_back(node);
                    }
                }
            }
        }
    }

    void UnselectAll(std::vector<HierarchyNode> &nodes)
    {
        for (HierarchyNode& child : nodes)
        {
            child.Selected = false;
            UnselectAll(child.Children);
        }
    }

    void DrawHierarchyElementRecursive(HierarchyNode& node, bool indent)
    {

        ImGuiTreeNodeFlags nodeFlags = baseTreeFlags;
        if (node.Selected)
            nodeFlags |= ImGuiTreeNodeFlags_Selected;

        if (node.Children.size() == 0)
            nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        node.Open = ImGui::TreeNodeEx(node.ID.data(), nodeFlags, node.Name.c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            currentSelection = &node;
            UnselectAll(hierarchy);
            node.Selected = true;
        }

        // FIXME: Leafs are considered open, but they should not. Should local ImGui code be changed?
        if (node.Children.size() == 0)
            node.Open = false;

        if (node.Open)
        {
            for (HierarchyNode &child : node.Children)
            {
                DrawHierarchyElementRecursive(child, true);
            }
            ImGui::TreePop();
        }
    }

    virtual void OnGUI(Window *window)
    {
        ObjectTypeLibrary &lib = ObjectTypeLibrary::Get();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowBgAlpha(1);
        if (ImGui::Begin(GetName(), &Visible, ImGuiWindowFlags_NoCollapse))
        {
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
            ImGuiViewport *viewport = ImGui::GetWindowViewport();

            ImGuiID dockspace_id = ImGui::GetID("ObjectExplorerDockSpace");
            ImGui::SetNextWindowBgAlpha(.0f);
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, nullptr);

            static auto first_time = true;
            if (first_time)
            {
                first_time = false;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                auto dock_id_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.045f, nullptr, &dockspace_id);
                auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.12f, nullptr, &dockspace_id);

                // we now dock our windows into the docking node we made above
                ImGui::DockBuilderDockWindow(ContentsName, dockspace_id);
                ImGui::DockBuilderDockWindow(FilterName, dock_id_top);
                ImGui::DockBuilderDockWindow(HierarchyName, dock_id_left);
                ImGui::DockBuilderFinish(dockspace_id);
            }

            ImGuiWindowClass noTab_class;
            noTab_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

            ImGui::SetNextWindowClass(&noTab_class);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 6));
            if (ImGui::Begin(FilterName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
            {
                BImGui::InputString("Filter", Filter);
            }
            ImGui::PopStyleVar();
            ImGui::End();

            ImGui::SetNextWindowClass(&noTab_class);
            if (ImGui::Begin(HierarchyName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
            {
                for (HierarchyNode &node : hierarchy)
                {
                    DrawHierarchyElementRecursive(node, false);
                }
            }
            ImGui::End();

            ImGui::SetNextWindowClass(&noTab_class);
            if (ImGui::Begin(ContentsName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
            {
                if(currentSelection) 
                {
                    
                }
            }
            ImGui::End();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
};