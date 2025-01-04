#pragma once

struct Vec4
{
    float x = 0, y = 0, z = 0, w = 0;

    Vec4(){}
    Vec4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

    Vec4 operator+(const Vec4& other) {
        Vec4 ret;
        ret.x = x + other.x;
        ret.y = y + other.y;
        ret.z = z + other.z;
        ret.w = w + other.w;
        return ret;
    }
};

struct Vec3
{
    float x = 0, y = 0, z = 0;

    Vec3(){}
    Vec3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}

    Vec3 operator+(const Vec3& other) {
        Vec3 ret;
        ret.x = x + other.x;
        ret.y = y + other.y;
        ret.z = z + other.z;
        return ret;
    }

    Vec3 operator*(const Vec3& other) {
        Vec3 ret;
        ret.x = x * other.x;
        ret.y = y * other.y;
        ret.z = z * other.z;
        return ret;
    }
};

struct Vec2
{
    float x = 0, y = 0;

    Vec2(){}
    Vec2(float _x, float _y) :x(_x), y(_y) {}

    Vec2 operator+(const Vec2& other) {
        Vec2 ret;
        ret.x = x + other.x;
        ret.y = y + other.y;
        return ret;
    }
};

struct IVec4
{
    int x = 0, y = 0, z = 0, w = 0;

    IVec4(){}
    IVec4(int _x, int _y, int _z, int _w) : x(_x), y(_y), z(_z), w(_w) {}

    IVec4 operator+(const IVec4& other) {
        IVec4 ret;
        ret.x = x + other.x;
        ret.y = y + other.y;
        ret.z = z + other.z;
        ret.w = w + other.w;
        return ret;
    }
};

struct IVec3
{
    int x = 0, y = 0, z = 0;

    IVec3(){}
    IVec3(int _x, int _y, int _z) :x(_x), y(_y), z(_z) {}

    IVec3 operator+(const IVec3& other) {
        IVec3 ret;
        ret.x = x + other.x;
        ret.y = y + other.y;
        ret.z = z + other.z;
        return ret;
    }
};

struct IVec2
{
    int x = 0, y = 0;

    IVec2(){}
    IVec2(int _x, int _y) :x(_x), y(_y) {}

    IVec2 operator+(const IVec2& other) {
        IVec2 ret;
        ret.x = x + other.x;
        ret.y = y + other.y;
        return ret;
    }
};