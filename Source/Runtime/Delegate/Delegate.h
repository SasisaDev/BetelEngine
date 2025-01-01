#pragma once

#include "DelegateInstance.h"
#include <memory>
#include <vector>

template <typename RetValType, typename... ArgTypes>
class Delegate
{
    std::shared_ptr<DelegateInstance<RetValType(ArgTypes...)>>instance;
public:

    template <typename UserClass>
    inline static Delegate<RetValType, ArgTypes...>
    CreateMember(
        UserClass* InUserObject,
        typename MemFuncPtr<false,
            UserClass,
            RetValType(ArgTypes...)
        >::Type InFunc)
    {
        Delegate<RetValType(ArgTypes...)> Result;
        Result.instance.reset(new MemDelegateInstance<false, UserClass, RetValType, ArgTypes...>(InUserObject, InFunc));
        return Result;
    }

    inline static Delegate<RetValType, ArgTypes...>
    CreateRaw(typename RawFuncPtr<RetValType(ArgTypes...)>::Type InFunc)
    {
        Delegate<RetValType(ArgTypes...)> Result;
        Result.instance.reset(new RawDelegateInstance<RetValType, ArgTypes...>(InFunc));
        return Result;
    }

    inline RetValType Execute(ArgTypes... Params)
    {
        return instance->Execute(Params...);
    }
};

template <typename... ArgTypes>
class MulticastDelegate {
    std::vector<std::shared_ptr<DelegateInstance<void(ArgTypes...)>>> delegates;
public:
    inline void BindRaw(typename RawFuncPtr<void(ArgTypes...)>::Type InFunc)
    {
        delegates.push_back(std::make_shared<RawDelegateInstance<void, ArgTypes...>>(InFunc));
    }

    template <typename UserClass>
    inline void BindMember(UserClass* InUserObject,
        typename MemFuncPtr<false,
            UserClass,
            void(ArgTypes...)
        >::Type InFunc)
    {
        delegates.push_back(std::make_shared<MemDelegateInstance<false, UserClass, void, ArgTypes...>>(InUserObject, InFunc));
    }

    inline void Broadcast(ArgTypes... Params)
    {
        for(auto delegate : delegates)
        { 
            delegate->Execute(Params...);
        }
    }
};