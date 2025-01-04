#pragma once

#include <vector>
#include "Entity.h"
#include "World.h"
#include <Core/Application/Application.h>
#include <GameFramework/Settings/GameSettings.h>
#include <Log/Logger.h>

#ifdef EDITOR
#   include <Editor/Editor.h>       
#endif

struct HitscanParams {
    std::vector<Entity*> IgnoreEntities;
    bool MultiScan = false;
};

struct HitResult {
    std::vector<Entity*> HitEntities;
    IVec2 Hitpoint = {};
    int Depth = 0;
};

class WorldUtility {
public:

    // TODO: World Raycast Test 
    static HitResult HitscanWorld(World* world, IVec2 hitpoint) {
        HitResult result;

        return result;
    }
    
    // TODO: Cam to World Projection
    static HitResult HitscanCamToWorld(World* world, float ViewportX, float ViewportY, float ViewportW, float ViewportH, HitscanParams& params) {
        IVec2 &CameraCenter = world->GetWorldCameraPosition();
#       ifdef EDITOR
        // TODO: Dynamic Editor Viewport Size
        Vec2 ViewportSize = {1280, 720};
        // If Viewport Texture Size >= Viewport Real Size, VPPGP == Zoom
        float ViewportPixelsPerGamePixels = Editor::Get()->ViewportZoom;
#       else
        const GameSettings* gSettings = GApplication->GetSettings()->GetOrDefault<GameSettings>();  
        Vec2 ViewportSize = {gSettings->PixelPerfectViewportWidth, gSettings->PixelPerfectViewportHeight};
        // If Viewport Texture Size < Viewport Real Size, VPPGP == RealViewportSize / GameViewportSize
        // TODO: Game VPPGP Implementation
        float ViewportPixelsPerGamePixels =  1;
#       endif

        // Pixel on a Viewport Texture, disregarding Camera Position
        IVec2 TargetPixel = {static_cast<int>((ViewportX + ViewportW / 2) / ViewportPixelsPerGamePixels), 
                             static_cast<int>((ViewportY + ViewportH / 2) / ViewportPixelsPerGamePixels)};

        // Real World Hitpoint
        IVec2 WorldHitpoint = TargetPixel + CameraCenter;

        LOGF(Log, LogHitscan, "VPPGP == %f, X = %d, Y = %d", ViewportPixelsPerGamePixels, WorldHitpoint.x, WorldHitpoint.y);
        
        return HitscanWorld(world, WorldHitpoint);
    }

    // TODO: World to Cam Projection
};