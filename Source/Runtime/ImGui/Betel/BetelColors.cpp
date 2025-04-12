#include "BetelColors.h"

#include <cstdlib>
#include <ctime>
#include <cmath>

ImVec4 BImGui::Colors::RandomColor(unsigned int seed)
{
    srand (seed);
    // FIXME: For some reason first rand is always very small, which causes all colors to be nearly red-less
    rand();
    int randnum = rand();
    float r = static_cast <float> (randnum) / static_cast <float> (RAND_MAX);
    randnum = rand();
    float g = static_cast <float> (randnum) / static_cast <float> (RAND_MAX);
    randnum = rand();
    float b = static_cast <float> (randnum) / static_cast <float> (RAND_MAX);
    return ImVec4(r, g, b, 1.0f);
}

float BImGui::Colors::GetBrightness(ImVec4 color)
{
    return std::sqrt(
        color.x * color.x * .241f +
        color.y * color.y * .691f +
        color.z * color.z * .068f);
}

ImVec4 BImGui::Colors::RandomBrightColor(unsigned int seed)
{
    ImVec4 randColor = RandomColor(seed);
    float brightness = GetBrightness(randColor);

    const float requiredBrightness = 0.75f;

    float brightnessCompensationCoefficient = requiredBrightness / brightness;

    return {randColor.x * brightnessCompensationCoefficient, 
            randColor.y * brightnessCompensationCoefficient, 
            randColor.z * brightnessCompensationCoefficient,
            1.0f};
}