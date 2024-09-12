#pragma once

struct Color
{
    float r = 0, g = 0, b = 0, a = 0;

    Color(){}
    Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
};