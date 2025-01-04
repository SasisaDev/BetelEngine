#pragma once

#include "Vector.h"
#include "Quaternion.h"

struct Transform
{
    IVec3 Location = {0, 0, 0};
    Vec3 Scale = {1, 1, 1};
    Quat Rotator;

    Transform(){}

    Transform operator+(Transform const& other) {
        Transform ret;
        ret.Location = Location + other.Location;
        ret.Scale = Scale + other.Scale;
        //ret.Rotator = Rotator + other.Rotator;
        return ret;
    }
};