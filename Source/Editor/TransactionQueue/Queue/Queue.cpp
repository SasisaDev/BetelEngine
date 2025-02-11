#include "Queue.h"

#include <cassert>
#include <memory>
#include <cstring>

void TransactionQueue::EraseFrame(uint64_t offset)
{
    uint64_t frameStart = offset;
    uint64_t frameSize = frames[frameStart];
    memset((char*)(memory.memory) + frameStart, 0x00, frameSize);

    frames.erase(frameStart);
}

std::vector<uint64_t> TransactionQueue::GetCollidingFrames(uint64_t offset, uint64_t size)
{
    std::vector<uint64_t> collisions;
    uint64_t leftRequiredMemory = size;

    for(auto it = frames.begin(); it != frames.end(); ++it)
    {
        // Continue if no collision found
        if(!((it->first >= offset) && (it->first <= offset+size)))
        {
            continue;
        }

        if(it->second >= leftRequiredMemory) 
        {
            collisions.push_back(it->first);
            break;
        } 
        else 
        {
            collisions.push_back(it->first);
            leftRequiredMemory -= it->second;
            continue;
        }
    }

    return collisions;
}

TransactionQueue::TransactionQueue(uint64_t memorySize)
{
    stackPointer = 0;

    memory.capacity = memorySize;
    memory.memory = new char[memorySize];
    assert(memory.memory != nullptr && "Failed allocating memory");
}

TransactionQueue::~TransactionQueue()
{
    delete[] memory.memory;
}

void TransactionQueue::StartFrame(uint64_t size)
{
    assert(size <= memory.capacity && "Frame size is too big");

    // Check if enough memory is available in the stack 
    if(stackPointer + size > memory.capacity){
        stackPointer = 0;
    }

    // Check if any other frame collides with new frame
    std::vector<uint64_t> collisions = GetCollidingFrames(stackPointer, size);

    // If collisions happen, erase colliding frames
    if(collisions.size() > 0)
    {
        for(uint64_t collision : collisions)
        {
            EraseFrame(collision);
        }
    }

    frameBase = stackPointer;
    frameEnd = frameBase + size;
}

void TransactionQueue::EndFrame()
{
    assert(stackPointer == frameEnd && "Ended Frame before reaching it's allocated end!");

    frames[frameBase] = frameEnd;
    stackPointer = frameEnd;
}

uint64_t TransactionQueue::AddStackPointer(uint64_t add)
{
    if((stackPointer + add) > memory.capacity) {
        stackPointer = (stackPointer + add) - memory.capacity;
    } else {
        stackPointer += add;
    }

    return stackPointer;
}

uint64_t TransactionQueue::SubStackPointer(uint64_t sub)
{
    if(stackPointer < sub) {
        stackPointer = memory.capacity - (sub - stackPointer);    
    } else {
        stackPointer -= sub;
    }

    return stackPointer;
}

void TransactionQueue::Push(void *data, uint16_t size)
{
    assert((stackPointer + size) <= frameEnd && "Trying to push data outside of frame's bounds. Did you forget to start new level or miscalculated it's size?");
    memcpy(memory.memory + stackPointer, data, size);
    stackPointer += size;
}
