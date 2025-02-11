#pragma once

#include <string_view>

class TransactionQueue;

class Transaction
{
protected:
    
public:
    static std::string_view GetID() {return "NONE";}
    static void Record(TransactionQueue* queue){}
    static void Undo(TransactionQueue* queue){}
};