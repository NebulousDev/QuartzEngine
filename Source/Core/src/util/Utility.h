#pragma once

#include "../Common.h"

namespace Quartz
{
	//////////////////////////////////////////////////////////////
	// Template Utility Functions
	//////////////////////////////////////////////////////////////

	/** A constant value */
	template<typename _ValueType, _ValueType val>
	struct CompileConstant
	{
		using ValueType = _ValueType;
		constexpr static ValueType value = val;
		constexpr operator ValueType() const noexcept { return value; }
	};

	//////////////////////////////////////////////////////////////

	using TrueType = CompileConstant<Bool8, true>;
	using FalseType = CompileConstant<Bool8, false>;

	//////////////////////////////////////////////////////////////

	/* A list of types */
	template<typename... Types>
	struct TypeList;

	template<typename... Types>
	struct TypeListSize : public CompileConstant<USize, sizeof...(Types)> {};

	template<typename... Types>
	struct TypeList
	{
		constexpr static USize size = TypeListSize<Types...>::value;
	};

	//////////////////////////////////////////////////////////////

	/* Checks if two types are equal */
	template<typename Type1, typename Type2>
	struct IsSameType : public FalseType {};

	template<typename Type>
	struct IsSameType<Type, Type> : public TrueType {};

	//////////////////////////////////////////////////////////////

	template<Bool8 cond, typename T = void>
	struct _EnableIf {};

	template<typename T>
	struct _EnableIf<true, T> { typedef T Type; };

	/* Enable template only if condition is true */
	/*
	template<Bool8 cond, typename T = void>
	using EnableIf = typename _EnableIf<cond, T>::Type;
	*/

	//////////////////////////////////////////////////////////////

	/*
		Applies a conditional statement with types.
		If value is true, Type will be the first type,
		if value is false, Type will be the second type
	*/
	template<Bool8 cond, typename TrueType, typename FalseType>
	struct Condition
	{
		using Type = TrueType;
	};

	template<typename TrueType, typename FalseType>
	struct Condition<false, TrueType, FalseType>
	{
		using Type = FalseType;
	};

	//////////////////////////////////////////////////////////////

	/*
		Applies a conditional statement with indices.
		If value is true, index will be the first value,
		if value is false, index will be the second value
	*/
	template<Bool8 value, USize trueValue, USize falseValue>
	struct ConditionIndex
	{
		constexpr static USize index = trueValue;
	};

	template<USize trueValue, USize falseValue>
	struct ConditionIndex<false, trueValue, falseValue>
	{
		constexpr static USize index = falseValue;
	};

	//////////////////////////////////////////////////////////////

	/*
		Applys a compare and fold left for each value 
	
		Note: FoldCompare requires at least one type.
	*/
	template<typename CompareFunc, typename Type, typename... Types>
	Type FoldCompare(CompareFunc comp, Type value, Types... values)
	{
		if constexpr (sizeof...(values) == 0)
		{
			return value;
		}
		else
		{
			Type recurValue = FoldCompare<CompareFunc, Types...>(comp, values...);
			return comp(value, recurValue) ? value : recurValue;
		}
	}
}