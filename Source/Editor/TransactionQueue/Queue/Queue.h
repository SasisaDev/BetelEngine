#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <optional>

/*
 * Transactions are placed in heap memory similiar to stack structure
 * Topmost position of a memory space must have a 32bit integer containing unique transaction type ID 
 * 
 * If memory goes out of bounds, stackPointer is allowed to overflow
 * 
 */
struct TransactionQueueMemory
{
    char* memory = nullptr;
    uint64_t capacity = 0;
};

class TransactionQueue
{
    TransactionQueueMemory memory;

    // Map of Frame starts and their sizes
    std::unordered_map<uint64_t, uint64_t> frames;

    uint64_t stackPointer = 0;
    uint64_t frameBase = 0;
    uint64_t frameEnd = 0;
protected:
    void EraseFrame(uint64_t offset);
    std::vector<uint64_t> GetCollidingFrames(uint64_t offset, uint64_t size);
    bool DoesFit(uint64_t offset, uint64_t size);
public:
    TransactionQueue(uint64_t memorySize);
    ~TransactionQueue();

    void StartFrame(uint64_t size);
    void EndFrame();

    uint64_t AddStackPointer(uint64_t add);
    uint64_t SubStackPointer(uint64_t sub);
    uint64_t GetStackPointer() const {return stackPointer;}
    uint64_t GetFrameBase() const {return frameBase;}
    
    void Push(void* data, uint16_t size);
    void Pop(void* data, uint16_t size);
};