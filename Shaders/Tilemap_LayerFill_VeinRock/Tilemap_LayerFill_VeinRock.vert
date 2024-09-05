#version 450

layout(set = 0, binding = 0) uniform SceneData {
    mat4x4 ViewProj;
};

layout(set = 0, binding = 1) uniform sampler2D Atlases[8];

struct TileData
{
    uint atlasID;
    vec2 position;
};

layout(std140, binding = 2) readonly buffer TilemapData {
    uint chunkWidth;
    uint tileSize;
    TileData tileData[];
};

void main()
{

}