#pragma once

#include <set>

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>

#include <imgui/imgui_internal.h>

struct HierarchyNode
{
    std::string Name;
    IPath Path;
    bool Selected = false;
    bool Open = false;
    std::vector<HierarchyNode *> Children;
};

class EditorAssetExplorer : public EditorToolkitWindow
{
    std::set<std::string> InvisibleDirectoryNames = {
        "i18n"
    };
protected:
    Text TabName = Text("EditorUI", "AssetExplorer", "TabName");
    std::string TranslatedName;

    const char *HierarchyName = "##AssetsHierarchy";
    const char *ContentsName = "##AssetsContents";

    std::vector<HierarchyNode *> hierarchy;
    HierarchyNode *currentSelection = nullptr;

    ImGuiTreeNodeFlags baseTreeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

public:
    EditorAssetExplorer()
    {
        TranslatedName = TabName.Get();
        UpdateHierarchy();
    }

    virtual const char* GetName() override {return TranslatedName.c_str();}

    void PopulateNodeRecursive(HierarchyNode *node, IDirectory *dir)
    {
        for (IDirectory *child : dir->GetChildren())
        {
            if (!child->IsDirectory())
                continue;

            if(InvisibleDirectoryNames.contains(child->GetPath().GetName()))
                continue;

            HierarchyNode *newNode = new HierarchyNode();
            newNode->Name = child->GetPath().GetName();
            newNode->Path = child->GetPath();
            PopulateNodeRecursive(newNode, child);
            node->Children.push_back(newNode);
        }
    }

    void UpdateHierarchy()
    {
        IDirectory *hierarchyDir = IPlatform::Get()->OpenLocalDirectory("Content", DIRECTORY_FLAG_RECURSIVE);

        if (!hierarchyDir || !hierarchyDir->Exists())
        {
            LOG(Fatal, LogAssetExplorer, "Failed to open ./Content/ folder, you should create it yourself if it doesn't exist!");
        }

        hierarchy.clear();

        HierarchyNode *gameNode = new HierarchyNode();
        gameNode->Name = "Game";
        gameNode->Path = hierarchyDir->GetPath();
        PopulateNodeRecursive(gameNode, hierarchyDir);
        hierarchy.push_back(gameNode);
    }

    void UnselectAll(std::vector<HierarchyNode *> nodes)
    {
        for (HierarchyNode *child : nodes)
        {
            child->Selected = false;
            UnselectAll(child->Children);
        }
    }

    void DrawHierarchyElementRecursive(HierarchyNode *node, bool indent)
    {

        ImGuiTreeNodeFlags nodeFlags = baseTreeFlags;
        if (node->Selected)
            nodeFlags |= ImGuiTreeNodeFlags_Selected;

        if (node->Children.size() == 0)
            nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        node->Open = ImGui::TreeNodeEx(node->Path.GetPath().c_str(), nodeFlags, node->Name.c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            currentSelection = node;
            UnselectAll(hierarchy);
            node->Selected = true;
        }

        // FIXME: Leafs are considered open, but they should not. Should local ImGui code be changed?
        if (node->Children.size() == 0)
            node->Open = false;

        if (node->Open)
        {
            for (HierarchyNode *child : node->Children)
            {
                DrawHierarchyElementRecursive(child, true);
            }
            ImGui::TreePop();
        }
    }

    virtual void OnGUI(Window *window)
    {
        AssetLibrary &lib = AssetLibrary::Get();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowBgAlpha(1);
        if (ImGui::Begin(GetName(), &Visible, ImGuiWindowFlags_NoCollapse))
        {
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
            ImGuiViewport *viewport = ImGui::GetWindowViewport();

            ImGuiID dockspace_id = ImGui::GetID("AssetExplorerDockSpace");
            ImGui::SetNextWindowBgAlpha(.0f);
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, nullptr);

            static auto first_time = true;
            if (first_time)
            {
                first_time = false;

                ImGui::DockBuilderRemoveNode(dockspace_id);
                ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
                ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

                auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);

                // we now dock our windows into the docking node we made above
                ImGui::DockBuilderDockWindow(ContentsName, dockspace_id);
                ImGui::DockBuilderDockWindow(HierarchyName, dock_id_left);
                ImGui::DockBuilderFinish(dockspace_id);
            }

            ImGuiWindowClass noTab_class;
            noTab_class.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_NoTabBar;

            ImGui::SetNextWindowClass(&noTab_class);
            if (ImGui::Begin(HierarchyName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
            {
                for (HierarchyNode *node : hierarchy)
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