#version 460 core

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec2 inUV;

layout(location=0) out vec2 texcoords;

layout(std140, set = 0, binding = 0) uniform WorldData {
    mat4 Projection;
    mat4 View;
    vec2 Position;
} worldData;

layout(push_constant) uniform ObjectData {
    ivec2 Position;
    ivec2 Size;
    int Rotation;
    int Depth;
} objectData;

void main() {
    int depth = 1;

    gl_Position = worldData.View * worldData.Projection * vec4(inPosition * objectData.Size + vec2(worldData.Position.x, worldData.Position.y) + vec2(objectData.Position.x, -objectData.Position.y), objectData.Depth, 1);
    texcoords = inUV;
}