#include "Render.h"

IRenderEngine* IRenderFactory::CreateEngine()
{
    return new IRenderEngine;
}