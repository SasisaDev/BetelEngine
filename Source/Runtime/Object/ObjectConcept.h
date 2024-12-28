#pragma once

#include <concepts>

class Object;
class ObjectType;

template<typename T>
concept ObjectClass = std::is_base_of<Object, T>::value;

template<typename T>
concept ObjectTypeClass = std::is_base_of<ObjectType, T>::value;