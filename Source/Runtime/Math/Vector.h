#pragma once

struct Vec4
{
    float x = 0, y = 0, z = 0, a = 0;

    Vec4(){}
    Vec4(float _x, float _y, float _z, float _a) : x(_x), y(_y), z(_z), a(_a) {}
};

struct Vec3
{
    float x = 0, y = 0, z = 0;

    Vec3(){}
    Vec3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
};

struct Vec2
{
    float x = 0, y = 0;

    Vec2(){}
    Vec2(float _x, float _y) :x(_x), y(_y) {}
};