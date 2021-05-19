#pragma once
#include "Array.h"

namespace Quartz
{
	template<class _Type>
	class ArrayStack : protected Array<_Type>
	{
	public:
		using Type				= _Type;
		using ArrayStackType	= ArrayStack<_Type>;
		using ArrayType			= Array<_Type>;

	public:
		ArrayStack() {}

		ArrayStack(const ArrayStackType& stack)
			: ArrayType(static_cast<ArrayType>(stack)) {}

		ArrayStack(const ArrayType& array)
			: ArrayType(array) {}

		void Push(const Type& value)
		{
			Array<Type>::PushBack(value);
		}

		void Push(Type&& value)
		{
			Array<Type>::PushBack(Move(value));
		}

		Type* Peek()
		{
			// TODO: Replace with asserts?
			if (Array<Type>::mSize == 0)
			{
				return nullptr;
			}

			return &Array<Type>::mpData[Array<Type>::mSize - 1];
		}

		void Pop()
		{
			// TODO: Replace with asserts?
			if (!Array<Type>::mSize == 0)
			{
				Array<Type>::Remove(Array<Type>::mSize - 1);
			}
		}

		using ArrayType::Contains;

		using ArrayType::begin;
		using ArrayType::end;

		using ArrayType::Data;
		using ArrayType::Size;
		using ArrayType::Capacity;
		using ArrayType::IsEmpty;
	};

	template<class Type>
	using Stack = ArrayStack<Type>;
}