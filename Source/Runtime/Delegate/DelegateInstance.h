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
  typename RawFuncPtr<RetType(ArgTypes...)>::Type function;
public:
  RawDelegateInstance(typename RawFuncPtr<RetType(ArgTypes...)>::Type InMethodPtr) : function(InMethodPtr) {}
 /**
  * Execute the delegate.
  If the function pointer
  is not valid, an error
  will occur.
  */
  virtual RetType Execute(ArgTypes... args) const override
  {
    return function(args...);
  }
};

// MemFuncPtr Delegate
template <bool Const, typename UserClass, typename RetType,
  typename... ArgTypes>
class MemDelegateInstance : public DelegateInstance<RetType(ArgTypes...)>
{
    //MemFuncPtr<UserClass, RetType, ArgTypes...> ptr;

    // FIXME: Make auto-deduction work
    typename MemFuncPtr<Const, UserClass, RetType(ArgTypes...)>::Type function;
    UserClass* user;
public:
  MemDelegateInstance(UserClass* InUserObject, typename MemFuncPtr<Const, UserClass, RetType(ArgTypes...)>::Type InMethodPtr) : user(InUserObject), function(InMethodPtr) {}

 /**
  * Execute the delegate.
  If the function pointer
  or user class is not valid, 
  an error will occur.
  */
  virtual RetType Execute(ArgTypes... args) const override
  {
    return (user->*function)(args...);
  }
};