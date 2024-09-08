#pragma once

template <typename FuncType>
struct RawFuncPtr;

template <typename RetType, typename... ArgTypes>
struct RawFuncPtr<RetType(ArgTypes...)> 
{
  typedef RetType (*Type)(ArgTypes...);
};

// Member Function Pointer
template <bool Const,
  typename Class,
  typename FuncType>
struct MemFuncPtr;

// Non-const Member Function Pointer
template <typename Class,
  typename RetType,
  typename... ArgTypes>
struct MemFuncPtr<false,
  Class,
  RetType(ArgTypes...)>
{
  typedef RetType(Class::* Type)(ArgTypes...);
};

// Const Member Function Pointer
template <typename Class,
  typename RetType,
  typename... ArgTypes>
struct MemFuncPtr<true,
  Class,
  RetType(ArgTypes...)>
{
  typedef RetType(Class::* Type)(ArgTypes...) const;
};