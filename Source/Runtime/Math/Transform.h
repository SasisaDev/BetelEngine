#pragma once

#include "Vector.h"
#include "Quaternion.h"

struct Transform
{
    Vec3 Location;
    Vec3 Scale;
    Quat Rotator;

    Transform(){}
};