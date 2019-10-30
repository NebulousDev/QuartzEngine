#pragma once

#include <type_traits>

template<typename... Types>
struct TypePack {};

template<typename... Types>
static constexpr bool IsTypePack = false;

template<typename... Types>
static constexpr bool IsTypePack<TypePack<Types...>> = true;

template<typename ReturnType, typename... Types>
using InvalidPack = typename std::enable_if<not IsTypePack<Types...>, ReturnType>::type;

template<typename ReturnType, typename Type>
using ValidPack = typename std::enable_if<IsTypePack<Type>, ReturnType>::type;