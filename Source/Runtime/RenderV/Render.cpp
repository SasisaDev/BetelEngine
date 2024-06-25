#include "Render.h"

IRenderFactory& IRenderFactory::Get()
{
    static IRenderFactory factory = IRenderFactory();
    return factory; 
}

IRenderEngine* IRenderFactory::CreateEngine()
{
    return new IRenderEngine;
}