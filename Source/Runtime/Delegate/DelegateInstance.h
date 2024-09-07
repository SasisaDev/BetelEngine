#pragma once

#include "DelegatePointers.h"

template <typename FuncType>
class DelegateInstance;


template <typename RetType,
  typename... ArgTypes>
class DelegateInstance
  <RetType(ArgTypes...)>
{
public:
 /**
  * Execute the delegate.
  If the function pointer
  is not valid, an error
  will occur.
  */
  virtual RetType Execute(ArgTypes...) const = 0;
};

// RawFuncPtr Delegate
template <typename RetType,
  typename... ArgTypes>
class RawDelegateInstance : public DelegateInstance<RetType(ArgTypes...)>
{
    //RawFuncPtr<RetType, ArgTypes...> ptr;
    // FIXME: Make auto-deduction work
    RawFuncPtr<RetType(ArgTypes...)> ptr;
public:
 /**
  * Execute the delegate.
  If the function pointer
  is not valid, an error
  will occur.
  */
  virtual RetType Execute(ArgTypes...) const override
  {
    return;
  }
};

// MemFuncPtr Delegate
template <typename UserClass, typename RetType,
  typename... ArgTypes>
class MemDelegateInstance : public DelegateInstance<RetType(ArgTypes...)>
{
    //MemFuncPtr<UserClass, RetType, ArgTypes...> ptr;

    // FIXME: Make auto-deduction work
    MemFuncPtr<false, UserClass, RetType(ArgTypes...)> ptr;
    UserClass* user;
public:
 /**
  * Execute the delegate.
  If the function pointer
  or user class is not valid, 
  an error will occur.
  */
  virtual RetType Execute(ArgTypes... args) const override
  {
    return ptr.function(args...);
  }
};