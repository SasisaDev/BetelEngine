#pragma once

#include "Queue/Queue.h"
#include "Queue/Transaction.h"

#include <Utility/Hash.h>

#include <unordered_map>
#include <functional>

struct TransactionFunctions
{
    std::function<uint64_t(Transaction*)> size;
    std::function<void(TransactionQueue*, Transaction*)> record;
    std::function<void(TransactionQueue*)> undo;
    TransactionFunctions() {}
    TransactionFunctions(const std::function<uint64_t(Transaction*)>& siz, 
                         const std::function<void(TransactionQueue*, Transaction*)>& rec, 
                         const std::function<void(TransactionQueue*)>& und) {
        size = siz;
        record = rec;
        undo = und;
    }
};

class TransactionContext
{
    TransactionQueue queue;
    std::unordered_map<uint32_t, TransactionFunctions> transactions;
public:
    TransactionContext(uint64_t memorySize);

    template <class TransactionType>
    bool RegisterTransaction()
    {
        uint32_t hash = HashStringView::Hash32(TransactionType::GetID());
        transactions[hash] = {TransactionType::Size, TransactionType::Record, TransactionType::Undo};
        return true;
    }

    template <class TransactionType>
    bool Record(TransactionType&& transaction) {
        uint32_t hash = HashStringView::Hash32(TransactionType::GetID());

        // Register transaction if not already registered
        if(!transactions.contains(hash)){
            transactions[hash] = {TransactionType::Size, TransactionType::Record, TransactionType::Undo};
        }

        queue.StartFrame(transactions[hash].size(&transaction) + sizeof(uint32_t));
        transactions[hash].record(&queue, &transaction);
        queue.Push(&hash, sizeof(hash));
        queue.EndFrame();
        
        return true;
    }

    bool Undo();

    inline TransactionQueue& GetQueue() {return queue;} 
};