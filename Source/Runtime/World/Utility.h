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
    static HitResult HitscanWorld(World* world, IVec2 hitpoint, const HitscanParams& params = HitscanParams()) {
        HitResult result;
        result.Hitpoint = hitpoint;

        for(const ObjectRef<Entity>& ent : world->GetEntities()) {
            IVec3 loc = ent.Get()->GetLocation();
            Vec3 bbox = ent.Get()->GetBoundingBox();
            bool isXHit = false;
            bool isYHit = false;
            // TODO: This is terrible, but okay for testing
            if(bbox.x > 0 ) {
                isXHit = hitpoint.x >= loc.x && hitpoint.x <= loc.x + bbox.x;
            } else {
                isXHit = hitpoint.x <= loc.x && hitpoint.x >= loc.x + bbox.x;
            }
            if(bbox.y > 0 ) {
                isYHit = hitpoint.y >= loc.y && hitpoint.y <= loc.y + bbox.y;
            } else {
                isYHit = hitpoint.y <= loc.y && hitpoint.y >= loc.y + bbox.y;
            }

            if(isXHit && isYHit) {
                for(Entity* ignoredEntity : params.IgnoreEntities) {
                    if(ignoredEntity == ent.Get()) {
                        continue;
                    }
                }
                result.HitEntities.push_back(ent.Get());

                if(!params.MultiScan){
                    break;
                }
            }
        }

        return result;
    }

    /*
     * Calculates and returns viewport coordinates from world coordinates
     * 
     * world must be an active world associated with some WorldRenderLayerRef
     */
    static glm::vec2 GetScreenSpaceFromWorldSpace(World* world, float ViewportX, float ViewportY, float ViewportW, float ViewportH, float X, float Y)
    {
        assert(world != nullptr);

        glm::vec4 clipSpacePos = world->GetSceneView().ProjectionMatrix * 
                                (world->GetSceneView().ViewMatrix * 
                                 glm::vec4(X - world->GetSceneView().ViewOrigin.x, 
                                           -Y + world->GetSceneView().ViewOrigin.y, 0.0f, 1.0f));

        if(clipSpacePos.w != 0)
        {
            clipSpacePos.x /= clipSpacePos.w;
            clipSpacePos.y /= clipSpacePos.w;
            clipSpacePos.z /= clipSpacePos.w;
        }

        glm::vec2 windowSpacePos = glm::vec2( ((clipSpacePos.x + 1.0f) / 2.0f) * ViewportW + ViewportX, ((clipSpacePos.y + 1.0f) / 2.0f) * ViewportH + ViewportY );

        //LOGF(Warning, LogWorldUtils, "world: %f, %f; clip: %f, %f; window: %f, %f", X, Y, clipSpacePos.x, clipSpacePos.y, windowSpacePos.x, windowSpacePos.y);

        return windowSpacePos;
    }
    
    /*
     * Calculates and returns world coordinates from viewport coordinates
     * 
     * world must be an active world associated with some WorldRenderLayerRef
     */
    static IVec2 GetWorldSpaceFromScreenSpace(World* world, float ViewportX, float ViewportY, float ViewportW, float ViewportH)
    {
        assert(world != nullptr);

        float ScreenPointX = (ViewportX / ViewportW) * 2.f - 1.f;
        float ScreenPointY = -(ViewportY / ViewportH) * 2.f + 1.f;

        // TODO: Change it to precalculated world->GetSceneView().ViewProjectionMatrix maybe?
        glm::mat4 ViewProj = world->GetSceneView().ProjectionMatrix * world->GetSceneView().ViewMatrix;
        glm::mat4 InverseViewProj = glm::inverse(ViewProj);

        glm::vec4 OriginPosition = glm::vec4(ScreenPointX, ScreenPointY, -1, 1) * InverseViewProj;
        OriginPosition.w = 1.0f / OriginPosition.w;
        OriginPosition.x *= OriginPosition.w;
        OriginPosition.y *= OriginPosition.w;
        OriginPosition.z *= OriginPosition.w;

        //LOGF(Log, LogHitscan, "sX = %f, sY = %f, X = %f, Y = %f", ScreenPointX, ScreenPointY, OriginPosition.x, OriginPosition.y);

        return {OriginPosition.x + world->GetSceneView().ViewOrigin.x, OriginPosition.y + world->GetSceneView().ViewOrigin.y};
    }
    
    // Creates Hitscan from Screen Position to World Position
    static HitResult HitscanCamToWorld(World* world, float ViewportX, float ViewportY, float ViewportW, float ViewportH, const HitscanParams& params = HitscanParams()) {
        return HitscanWorld(world, GetWorldSpaceFromScreenSpace(world, ViewportX, ViewportY, ViewportW, ViewportH), params);
    }

    // TODO: World to Cam Projection
};