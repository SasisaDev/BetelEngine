#pragma once

#include "Queue/Queue.h"
#include "Queue/Transaction.h"

#include <Utility/Hash.h>

#include <unordered_map>
#include <functional>

struct TransactionFunctions
{
    std::function<void(TransactionQueue*)> record;
    std::function<void(TransactionQueue*)> undo;
    TransactionFunctions() = delete;
    TransactionFunctions(const std::function<void(TransactionQueue*)>& rec, const std::function<void(TransactionQueue*)>& und) {
        record = rec;
        undo = und;
    }
};

class TransactionContext
{
    TransactionQueue queue;
    std::unordered_map<uint64_t, TransactionFunctions> transactions;
public:
    TransactionContext(uint64_t memorySize);

    template <class TransactionType>
    bool RegisterTransaction()
    {
        uint64_t hash = Hash::Hash64(TransactionType::GetID());
        transactions[hash] = {TransactionType::Record, TransactionType::Undo};
        return true;
    }

    bool Record();
    bool Undo();

    inline TransactionQueue& GetQueue() {return queue;} 
};