#include "Tilemap.h"
#include <Object/ObjectLibrary.h>
#include <Object/ObjectTypeLibrary.h>
#include <World/World.h>

bool EntTilemapType::bRegistered = ObjectTypeLibrary::Get().RegisterObjectType<EntTilemapType>(EntTilemap::GetStaticType());

void EntTilemap::RemoveLayer(size_t index)
{
   delete layers[index];
   layers.erase(layers.begin() + index); 
}

/*ObjectRef<EntTilemapChunk>& EntTilemap::CreateChunk(IVec2 position)
{
    EntTilemapChunk* chunk = GetWorld()->Spawn<EntTilemapChunk>(std::to_string(position.x) + "," + std::to_string(position.y));
    for(TilemapLayer* layer : layers) {
        chunk->layersData.push_back(layer->CreateLayerData());
    }
    chunks[position] = {ObjectRef<EntTilemapChunk>(chunk->GetID())}; 
    
    return chunks[position].Chunk; 
}*/