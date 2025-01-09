#pragma once

#include <concepts>
#include <type_traits>

template<typename BaseT, typename DerivedT>
concept Derived = std::is_base_of<BaseT, DerivedT>::value;