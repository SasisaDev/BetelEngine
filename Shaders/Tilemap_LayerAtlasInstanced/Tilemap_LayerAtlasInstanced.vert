#version 450

// It accepts simple single quad model of size 1;1
layout(location = 0) in vec2 inPosition;
// UVs same as quad
layout(location = 1) in vec2 inUV;

// Passes texcoords to frag shader. Should be modified by vert shader to point to correct position in atlas 
layout(location=0) out vec2 texcoords;

layout(std140, set = 0, binding = 0) uniform WorldData {
    mat4 Projection;
    mat4 View;
    vec2 CameraPosition;
} worldData;

// This data consists of common data for all the Tilemap entity 
layout(std140, binding = 1) readonly buffer TilemapData {
    // We expect amount of instances(tiles) to be ChunkWidth ^ 2
    uint ChunkWidth;
    uint TileSize;
} tilemapData;

struct TileData
{
    uint SpriteX;
    uint SpriteY;
    uint SpriteWidth;
    uint SpriteHeight;
};

// Current Tile data
layout(std140, binding = 3) readonly buffer LayerData {
    // We expect this array to be of size ChunkWidth ^ 2
    TileData tiles[];
} layerData;

void main()
{

}