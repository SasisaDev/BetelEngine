#pragma once

#include <string_view>

class TransactionQueue;

class Transaction
{
protected:
    
public:
    static std::string_view GetID() {return "NONE";}
    static uint64_t Size(Transaction* transaction) {return 0;}
    static void Record(TransactionQueue* queue, Transaction* transaction){}
    static void Undo(TransactionQueue* queue){}
};