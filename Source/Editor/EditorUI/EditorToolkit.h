#pragma once

#include <Toolkit/Toolkit.h>
#include <imgui.h>
#include <Log/Logger.h>

#include "Windows/SceneOutliner/SceneOutliner.h"
#include "Windows/Viewport/Viewport.h"
#include "Windows/LogViewer/LogViewer.h"
#include "Windows/Translator/Translator.h"
#include "Windows/MainToolbar/MainToolbar.h"
#include "Windows/Settings/GameSettings.h"
#include "Windows/AssetExplorer/AssetExplorer.h"
#include "Windows/ObjectExplorer/ObjectExplorer.h"

#include <EditorUI/WindowLibrary/BetelImages.h>

class EditorToolkitBase : public EditorToolkit {
    bool firstInitialization = true;

    EditorSceneOutliner sceneOutliner;
    EditorViewport gameViewport;
    EditorLogViewer logViewer;
    EditorAssetExplorer assetExplorer;
    EditorObjectExplorer objectExplorer;

    EditorTranslator translator;

    EditorMainToolbar mainToolbar;

    EditorGameSettings gameSettings;
public:
    ~EditorToolkitBase() {
        BImGui::UnloadEditorImages();
    }
    
    virtual void OnGUI(Window* window) override {
        
        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Create")) { 
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) { 
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                }
                if (ImGui::MenuItem("Save as..")) { 
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Game Settings")) { 
                    gameSettings.Visible = !gameSettings.Visible;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools")) {
                if (ImGui::MenuItem("Translator")) { 
                    translator.Visible = !translator.Visible;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View")) {
                if (ImGui::MenuItem("Viewport")) { 
                    gameViewport.Visible = !gameViewport.Visible;
                }
                if (ImGui::MenuItem("Asset Explorer")) { 
                    assetExplorer.Visible = !assetExplorer.Visible;
                }
                if (ImGui::MenuItem("Scene Outliner")) { 
                    sceneOutliner.Visible = !sceneOutliner.Visible;
                }
                if (ImGui::MenuItem("Details")) { 
                }
                if (ImGui::MenuItem("Log Viewer")) { 
                    logViewer.Visible = !logViewer.Visible;
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiWindowFlags host_window_flags = 0;
        host_window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
        host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        host_window_flags |= ImGuiWindowFlags_NoBackground;

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Window", nullptr, host_window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("DockSpace");
        ImGui::SetNextWindowBgAlpha(.0f);
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags, nullptr);
        ImGui::End();

        // TODO: Save layouts
        if (firstInitialization)
        {
            firstInitialization = false;

            // Initialize BImGui Editor Images
            BImGui::LoadEditorImages();

            ImGui::DockBuilderRemoveNode(dockspace_id);
            ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
            ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

            auto dock_id_top = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Up, 0.07f, nullptr, &dockspace_id);
            auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.25f, nullptr, &dockspace_id);
            auto dock_id_right_bottom = ImGui::DockBuilderSplitNode(dock_id_right, ImGuiDir_Down, 0.65f, nullptr, &dock_id_right);
            auto dock_id_bottom = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.25f, nullptr, &dockspace_id);

            // we now dock our windows into the docking node we made above
            ImGui::DockBuilderDockWindow(gameViewport.GetName(), dockspace_id);
            ImGui::DockBuilderDockWindow(mainToolbar.GetName(), dock_id_top);
            ImGui::DockBuilderDockWindow(sceneOutliner.GetName(), dock_id_right);
            ImGui::DockBuilderDockWindow("Details", dock_id_right_bottom);
            ImGui::DockBuilderDockWindow(logViewer.GetName(), dock_id_bottom);
            ImGui::DockBuilderDockWindow(assetExplorer.GetName(), dock_id_bottom);

            ImGui::DockBuilderFinish(dockspace_id);
        }

        // Must be drawn always?
        mainToolbar.DrawGUI(window);

        // Utility
        sceneOutliner.DrawGUI(window);
        gameViewport.DrawGUI(window);
        logViewer.DrawGUI(window);
        assetExplorer.DrawGUI(window);
        objectExplorer.DrawGUI(window);

        // Windows
        translator.DrawGUI(window);
        gameSettings.DrawGUI(window);

        ImGui::Begin("Details", 0, ImGuiWindowFlags_NoCollapse);ImGui::End();
    }
};