#pragma once

#include "Vector.h"
#include "Quaternion.h"

struct Transform
{
    IVec3 Location;
    Vec3 Scale;
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