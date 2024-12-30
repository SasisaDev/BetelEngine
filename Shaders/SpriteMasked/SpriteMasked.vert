#version 460 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;

layout(location=0) out vec2 texcoords;

layout(std140, set = 0, binding = 0) uniform WorldData {
    mat4 Projection;
    mat4 View;
    vec2 Position;
} worldData;

void main() {
    vec2 size = {150, 100};
    int depth = 1;

    gl_Position = worldData.View * worldData.Projection * vec4(inPosition * size + vec2(worldData.Position.x, worldData.Position.y), -10., 1); 
    texcoords = inUV;
}