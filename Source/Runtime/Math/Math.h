#pragma once

#include <glm/glm.hpp>

struct MathUtility
{
    static inline bool BoxPointCollide(glm::vec4 box, glm::vec2 point)
    {
        return point.x >= box.x && point.y >= box.y 
               && point.x <= box.z && point.y <= box.w;
    }
};