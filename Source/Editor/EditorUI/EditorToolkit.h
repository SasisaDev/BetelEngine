#pragma once

#include <Toolkit/Toolkit.h>
#include <imgui.h>
#include <Log/Logger.h>

#include "Windows/SceneOutliner/SceneOutliner.h"
#include "Windows/Viewport/Viewport.h"
#include "Windows/LogViewer/LogViewer.h"
#include "Windows/Translator/Translator.h"
#include "Windows/MainToolbar/MainToolbar.h"

class EditorToolkitBase : public EditorToolkit {
    EditorSceneOutliner sceneOutliner;
    EditorViewport gameViewport;
    EditorLogViewer logViewer;
    EditorTranslator translator;
    EditorMainToolbar mainToolbar;
public:
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

        // Must be drawn always?
        mainToolbar.DrawGUI(window);

        sceneOutliner.DrawGUI(window);
        gameViewport.DrawGUI(window);
        logViewer.DrawGUI(window);


        translator.DrawGUI(window);

        ImGui::Begin("Assets Explorer", 0, ImGuiWindowFlags_NoCollapse);ImGui::End();
        ImGui::Begin("Details", 0, ImGuiWindowFlags_NoCollapse);ImGui::End();
    }
};