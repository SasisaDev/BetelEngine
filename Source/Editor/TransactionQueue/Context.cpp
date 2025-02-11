#include "Context.h"
#include <cassert>

TransactionContext::TransactionContext(uint64_t memorySize)
    : queue(memorySize)
{

}

bool TransactionContext::Undo()
{
    return true;
}