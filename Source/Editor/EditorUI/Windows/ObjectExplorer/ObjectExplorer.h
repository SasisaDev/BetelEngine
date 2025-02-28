#pragma once

#include <set>

#include <Toolkit/ToolkitWindow.h>
#include <Core/Application/Application.h>
#include <Object/ObjectTypeLibrary.h>

#include <imgui/imgui_internal.h>
#include <ImGui/Betel/BetelInputs.h>
#include <ImGui/Betel/BetelDeferred.h>
#include <ImGui/Betel/BetelDrag.h>
#include <EditorUI/ObjectEditViews/ObjectEditViewsFactory.h>

class EditorObjectExplorer : public EditorToolkitWindow
{
    bool firstInitialization = true;

    struct HierarchyNode
    {
        std::string Name;
        std::string ID;
        std::string Parent;
        bool Selected = false;
        bool Open = false;
        std::vector<HierarchyNode> Children;
    };

    struct ObjectDeleterTask : public BImGui::DeferredTask
    {
        uint32_t ID = 0;
        bool ShouldCleanup = false;
        bool ShouldSelfDestruct = false;

        bool Perform() override {
            if(!ShouldCleanup) {
                return ShouldSelfDestruct;
            }

            if(ID != 0) {
                GEngine->GetObjectLibrary()->DestroyObject(ID);
                ID = 0;
            }

            ShouldCleanup = false;

            return ShouldSelfDestruct;
        }
    };

    ObjectDeleterTask* deleterTask;
    bool bDeletedObject = false;
protected:
    Text TabName = Text("EditorUI.ObjectExplorer.TabName");
    std::string TranslatedName;

    ImTextureID BlankTypeIcon;

    const char *HierarchyName = "##ObjectsHierarchy";
    const char *ContentsName = "##ObjectsContents";
    const char *FilterName = "##ObjectsFilter";
    std::string Filter;

    std::vector<HierarchyNode> hierarchy;
    HierarchyNode *currentSelectedType = nullptr;
    HierarchyNode *previousSelection = nullptr;

    ImGuiTreeNodeFlags baseTreeFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

    std::vector<ObjectDescriptor*> displayedObjects;

    std::unique_ptr<EditorToolkitWindow> editView;

    bool bObjectContextMenu = false;
public:
    EditorObjectExplorer()
    {
        deleterTask = BImGui::CreateDeferredTask<ObjectDeleterTask>();

        TranslatedName = TabName.Get();
        UpdateHierarchy();
    }

    ~EditorObjectExplorer() 
    {
        deleterTask->ShouldSelfDestruct = true;
    }

    virtual const char *GetName() override { return TranslatedName.c_str(); }

    void UpdateHierarchy()
    {
        ObjectTypeLibrary &lib = ObjectTypeLibrary::Get();
        auto map = lib.GetObjectTypes();

        // Clear outdated hierarchy
        hierarchy.clear();

        // Add All category
        HierarchyNode allNode;
        allNode.ID = "__ALL__";
        allNode.Name = "All*";
        hierarchy.push_back(allNode);

        // Search through the whole types map to build hierarchy
        for (auto it = map.begin(); it != map.end(); it++)
        {
            if (!it->second->ShowInEditor() || it->second->IsEntity())
            {
                continue;
            }

            HierarchyNode node;
            node.ID = it->first;
            node.Name = it->second->DisplayName();

            // Add to the global hierarchy if has no parent
            // FIXME: Or to parent if it has a parent
            if (it->second->GetParent().empty())
            {
                hierarchy.emplace_back(node);
            }
            else
            {
                for (HierarchyNode &parent : hierarchy)
                {
                    if (parent.ID == it->second->GetParent())
                    {
                        parent.Children.emplace_back(node);
                    }
                }
            }
        }
    }

    void UpdateDisplayedObjects()
    {
        if(!currentSelectedType){
            displayedObjects.clear();
            return; 
        }

        if (currentSelectedType->ID == "__ALL__")
        {
            // Display all objects
            displayedObjects = GEngine->GetObjectLibrary()->GetAllObjectDescriptors(true);
        }
        else
        {
            // Display objects of category
            displayedObjects = GEngine->GetObjectLibrary()->GetObjectDescriptorsOfTypeID(currentSelectedType->ID, true);
        }
        
        // TODO: Implement name filtering
    }

    void UnselectAll(std::vector<HierarchyNode> &nodes)
    {
        for (HierarchyNode &child : nodes)
        {
            child.Selected = false;
            UnselectAll(child.Children);
        }
    }

    void DrawHierarchyElementRecursive(HierarchyNode &node, bool indent)
    {

        ImGuiTreeNodeFlags nodeFlags = baseTreeFlags;
        if (node.Selected)
            nodeFlags |= ImGuiTreeNodeFlags_Selected;

        if (node.Children.size() == 0)
            nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        node.Open = ImGui::TreeNodeEx(node.ID.data(), nodeFlags, node.Name.c_str());
        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
        {
            currentSelectedType = &node;
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

    void StartObjectEditing(ObjectDescriptor* desc) {
        if(editView.get() == nullptr) {
            const std::string& type = desc->type;
            editView.reset();
            editView = ObjectEditorViewsFactory::CreateEditView(type, GEngine->GetObjectLibrary()->LoadObject(desc->id));
        }
    }
    
    ObjectDescriptor* CreateNewObject() {
        Object* object = GEngine->GetObjectLibrary()->CreateObjectFromTypeID(currentSelectedType->ID, ObjectTypeLibrary::Get().GetObjectType(currentSelectedType->ID)->DisplayName().data(), false);
        assert(object != nullptr && "Object Library returned nullptr on object creation. It may happen due to ObjectType not existing.");
        return GEngine->GetObjectLibrary()->GetObjectDescriptor(object->GetID());
    }

    void DeleteObject(ObjectDescriptor* desc) {
        deleterTask->ID = desc->id;
        deleterTask->ShouldCleanup = true;
        bDeletedObject = true;
    }

    void DrawObjectPopup(ObjectDescriptor* desc)
    {
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            StartObjectEditing(desc);
        }

        bObjectContextMenu = false;
            
        if(editView.get() != nullptr){
            return;
        }

        if(ImGui::BeginPopupContextItem())
        {
            bObjectContextMenu = true;

            ImGui::Selectable("Create");
            if(ImGui::Selectable("Edit")) {
                StartObjectEditing(desc);
            }
            if(ImGui::Selectable("Delete")) {
                DeleteObject(desc);
            }
            ImGui::Separator();
            ImGui::Selectable("Place in level");
            ImGui::EndPopup();
        }
    }

    void DrawObject(ObjectDescriptor* desc, float NameWidth, float TypeWidth) 
    {
        const float ImageSize = 16;
        const float Padding = 8;

        std::string hexID;
        hexID.resize(2+8);
        sprintf(hexID.data(), "0x%08X", desc->id);

        std::string fullName = desc->name + " (" + hexID + ")"; 

        ImGuiWindow* window = ImGui::GetCurrentWindowRead();
        ImVec2 WindowPos = window->DC.CursorPos;

        float InitialPosX = ImGui::GetCursorPosX();
        float InitialPosY = ImGui::GetCursorPosY();
        ImGui::Selectable((std::string("##") + fullName).c_str());
        if(ImGui::BeginDragDropSource())
        {
            //ImGui::SetDragDropPayload("ObjectID", &desc->id, ImGuiCond_Once);
            BImGui::Drag::Begin(desc->id);
            ImGui::EndDragDropSource();
        }

        DrawObjectPopup(desc);

        float LineEndPosY = ImGui::GetCursorPosY();
        ImGui::SameLine();
        float LineEndPosX = ImGui::GetCursorPosX();

        // Draw Image
        // TODO: Implement real icons for types
        ImGui::SetCursorPosX(InitialPosX + 4);
        ImGui::SetCursorPosY(InitialPosY + 2);
        ImGui::Image(BlankTypeIcon, ImVec2(ImageSize - 4, ImageSize - 4));

        // Draw Name
        ImGui::SameLine();
        ImGui::SetCursorPosX(InitialPosX + ImageSize + Padding);
        ImGui::SetCursorPosY(InitialPosY);

        ImGui::PushClipRect(ImVec2(InitialPosX, InitialPosY), ImVec2(WindowPos.x + LineEndPosX + NameWidth, WindowPos.y + LineEndPosY), true);
        ImGui::TextUnformatted(fullName.c_str());
        ImGui::PopClipRect();

        ImGui::SameLine();
        ImGui::SetCursorPosX(InitialPosX + LineEndPosX + NameWidth + Padding);

        //ImGui::PushClipRect(ImVec2(InitialPosX, InitialPosY), ImVec2(WindowPos.x + LineEndPosX + TypeWidth, WindowPos.y + LineEndPosY), true);
        ImGui::TextUnformatted(desc->type.c_str());
        //ImGui::PopClipRect();
    }

    void DrawObjects()
    {
        float NameColumnWidth = 0, TypeColumnWidth = 0;
        if(ImGui::BeginTable("##ObjectsContainer", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable))
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::TextUnformatted("Name");
            NameColumnWidth = ImGui::GetColumnWidth(0);
            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted("Type");
            TypeColumnWidth = ImGui::GetColumnWidth(1);
            ImGui::EndTable();
        }
        
        for(ObjectDescriptor* obj : displayedObjects)
        {
            DrawObject(obj, NameColumnWidth, TypeColumnWidth);
        }
    }

    virtual void OnGUI(Window *window)
    {
        if(firstInitialization) {
            firstInitialization = false;

            // TODO: Change to real icon
            BlankTypeIcon = BImGui::GetEdImage(BImGui::Img::Visibility32Icon);
        }

        if(bDeletedObject) {
            UpdateDisplayedObjects();
            bDeletedObject = false;
        }

        ObjectTypeLibrary &lib = ObjectTypeLibrary::Get();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowBgAlpha(1);
        ImGui::SetNextWindowSize(ImVec2(350, 550), ImGuiCond_Once);
        if (ImGui::Begin(GetName(), &Visible, ImGuiWindowFlags_NoCollapse))
        {
            ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDocking;
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

                if(previousSelection != currentSelectedType)
                {
                    UpdateDisplayedObjects();
                    previousSelection = currentSelectedType;
                }
            }
            ImGui::End();

            ImGui::SetNextWindowClass(&noTab_class);
            if (ImGui::Begin(ContentsName, 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
            {
                DrawObjects();
            }
            ImGui::End();
            
            if(bObjectContextMenu == false && currentSelectedType && currentSelectedType->ID != "__ALL__"){
                if(ImGui::BeginPopupContextItem())
                {
                    if(ImGui::Selectable("Create"))
                    {
                        StartObjectEditing(CreateNewObject());
                        UpdateDisplayedObjects();
                    }
                    ImGui::EndPopup();
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();

        if(editView.get()) {
            editView->DrawGUI(window);
            if(!editView->Visible) {
                editView.reset();
            }
        }
    }
};