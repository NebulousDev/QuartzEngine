#pragma once

#include "../Common.h"
#include "Utility.h"

namespace Quartz
{
	//////////////////////////////////////////////////////////////
	// Tuple
	//////////////////////////////////////////////////////////////

	/** A single tuple value */
	template<USize index, typename Type>
	class TupleValue
	{
	private:
		Type value;

	public:
		constexpr TupleValue(const Type& value)
			: value(value) { }

		constexpr TupleValue(Type&& value)
			: value(Move(value)) { }

		constexpr Type& Value()
		{
			return value;
		}
	};

	/** Base class type for Tuple */
	template<USize index, typename... Types>
	struct TupleBase {};

	template<USize index, typename Type, typename... Types>
	struct TupleBase<index, Type, Types...>
		: public TupleValue<index, Type>, public TupleBase<index + 1, Types...>
	{
		template<typename ValType, typename... ValTypes>
		constexpr TupleBase(ValType&& value, ValTypes&&... values)
			: TupleValue<index, ValType>(std::forward<ValType>(value)),
			TupleBase<index + 1, ValTypes...>(std::forward<ValTypes>(values)...) {}
	};

	////////////////////////////////////////////////////////////////////////

	/**
		Retrieves a Tuple element by type

		Note: This utility will return *only the first element of that type*,
		and will not work with tuples containing more than one of the same
		element type.
	*/
	template<USize index, typename _Type, typename... Types>
	struct TupleGetType
	{
		using Type = typename TupleGetType<index - 1, Types...>::Type;
	};

	template<typename _Type, typename... Types>
	struct TupleGetType<0, _Type, Types...>
	{
		using Type = _Type;
	};

	////////////////////////////////////////////////////////////////////////
	
	/* Retrieves a Tuple element by index */
	template<typename Search, USize index, typename... Types>
	struct TupleGetIndex;

	template<typename Search, USize _index, typename Type, typename... Types>
	struct TupleGetIndex<Search, _index, Type, Types...>
	{
		constexpr static USize index = ConditionIndex<IsSameType<Search, Type>::value, _index, TupleGetIndex<Search, _index + 1, Types...>::index>::index;
	};

	template<typename Search, USize _index, typename Type>
	struct TupleGetIndex<Search, _index, Type>
	{
		constexpr static USize index = ConditionIndex<IsSameType<Search, Type>::value, _index, -1>::index;
	};

	////////////////////////////////////////////////////////////////////////

	/**
		A compile-time storage type for a tuple of elements
		
		Note: A Tuple must contain at least one type
	*/
	template<typename Type, typename... Types>
	struct Tuple : public TupleBase<0, Type, Types...>
	{
		template<typename... ValTypes>
		constexpr Tuple(ValTypes&&... values)
			: TupleBase<0, Type, Types...>(std::forward<ValTypes>(values)...) {}

		template<USize index>
		constexpr auto Get()
		{
			return static_cast<TupleValue<index, typename TupleGetType<index, Type, Types...>::Type>>(*this).Value();
		}

		template<typename _Type>
		constexpr auto Get()
		{
			return static_cast<TupleValue<TupleGetIndex<_Type, 0, Type, Types...>::index, _Type>>(*this).Value();
		}

		constexpr USize Size() const
		{
			return sizeof...(Types) + 1;
		}
	};

}