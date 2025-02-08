#pragma once

#include <vector>
#include "Entity.h"
#include "World.h"
#include <Core/Application/Application.h>
#include <World/RenderLayer/WorldRenderLayer.h>
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
        result.Hitpoint = hitpoint;

        for(const ObjectRef<Entity>& ent : world->GetEntities()) {
            IVec3 loc = ent.Get()->GetLocation();
            Vec3 bbox = ent.Get()->GetBoundingBox();
            bool isXHit = false;
            bool isYHit = false;
            // This is terrible, but okay for testing
            if(bbox.x > 0 ) {
                isXHit = hitpoint.x >= loc.x && hitpoint.x <= bbox.x;
            } else {
                isXHit = hitpoint.x <= loc.x && hitpoint.x >= bbox.x;
            }
            if(bbox.y > 0 ) {
                isYHit = hitpoint.y >= loc.y && hitpoint.y <= bbox.y;
            } else {
                isYHit = hitpoint.y <= loc.y && hitpoint.y >= bbox.y;
            }

            if(isXHit && isYHit) {
                result.HitEntities.push_back(ent.Get());
                // TODO: Multiscan, etc.
                break;
            }
        }

        return result;
    }
    
    // Creates Hitscan from Screen Position to World Position
    static HitResult HitscanCamToWorld(World* world, float ViewportX, float ViewportY, float ViewportW, float ViewportH, HitscanParams& params) {
        WorldRenderLayerGPUStorage renderData = world->GetWorldRenderLayerRef()->GetSceneData();

        float ScreenPointX = (ViewportX / ViewportW) * 2.f - 1.f;
        float ScreenPointY = -(ViewportY / ViewportH) * 2.f + 1.f;

        glm::mat4 ViewProj = renderData.ProjectionMatrix * renderData.ViewMatrix;
        glm::mat4 InverseViewProj = glm::inverse(ViewProj);

        glm::vec4 OriginPosition = glm::vec4(ScreenPointX, ScreenPointY, -1, 1) * InverseViewProj;
        OriginPosition.w = 1.0f / OriginPosition.w;
        OriginPosition.x *= OriginPosition.w;
        OriginPosition.y *= OriginPosition.w;
        OriginPosition.z *= OriginPosition.w;

        IVec2 WorldHitpoint = {OriginPosition.x - renderData.CameraPosition.x, OriginPosition.y + renderData.CameraPosition.y};

        //LOGF(Log, LogHitscan, "sX = %f, sY = %f, X = %f, Y = %f", ScreenPointX, ScreenPointY, OriginPosition.x, OriginPosition.y);
        
        return HitscanWorld(world, WorldHitpoint);
    }

    // TODO: World to Cam Projection
};