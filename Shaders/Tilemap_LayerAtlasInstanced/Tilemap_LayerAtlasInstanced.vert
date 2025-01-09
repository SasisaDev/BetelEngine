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
    vec4 SpriteUV;
};

// Current Tile data
layout(std140, binding = 3) readonly buffer LayerData {
    ivec2 AtlasDimensions;
    ivec2 ChunkPosition;
    uint LayerDepth;
    // We expect this array to be of size ChunkWidth ^ 2
    TileData Tiles[];
} layerData;

void main()
{
    ivec2 sizedTilePosition = ivec2(int(inPosition.x * tilemapData.TileSize), int(inPosition.y * tilemapData.TileSize));

    // Move tile according to it's position in chunk
    sizedTilePosition += ivec2((gl_InstanceIndex % tilemapData.ChunkWidth) * tilemapData.TileSize, 
                                floor(gl_InstanceIndex / tilemapData.ChunkWidth) * tilemapData.TileSize);
    
    // Here we apply position of Tile, according to the chunk 
    ivec2 positionedTilePosition = sizedTilePosition + layerData.ChunkPosition;

    gl_Position = worldData.Projection * worldData.View * vec4(positionedTilePosition + worldData.CameraPosition, layerData.LayerDepth, 1);

    vec4 SpriteUV = layerData.Tiles[gl_InstanceIndex].SpriteUV;
    texcoords = vec2((inUV.x > 0) ? SpriteUV.y : SpriteUV.x, 
                     (inUV.y > 0) ? SpriteUV.w : SpriteUV.z) 
                     // Divide resulting vector by Atlas Dimensions to get UVs in range (0;1)
                     / layerData.AtlasDimensions;
}