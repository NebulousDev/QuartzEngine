#pragma once
#include "Array.h"

namespace Quartz
{
	template<class Type>
	class Stack : private Array<Type>
	{
	public:
		Stack();
		Stack(const Array<Type>& array);

		void Push(const Type& value);
		void Push(Type&& value);

		Type& Peek();

		Type Pop();

		using Array<Type>::begin;
		using Array<Type>::end;

		FORCE_INLINE Bool8 IsEmpty() const { return Array<Type>::mSize == 0; }
		FORCE_INLINE UInt32 GetSize() const { return Array<Type>::mSize; }
	};

	template<class Type>
	Stack<Type>::Stack()
	{
		// Nothing
	}

	template<class Type>
	Stack<Type>::Stack(const Array<Type>& array)
		: Array<Type>(array)
	{
		// Nothing
	}

	template<class Type>
	void Stack<Type>::Push(const Type& value)
	{
		Array<Type>::PushBack(value);
	}

	template<class Type>
	void Stack<Type>::Push(Type&& value)
	{
		Array<Type>::PushBack(value);
	}

	template<class Type>
	Type& Stack<Type>::Peek()
	{
		return Array<Type>::mpData[mSize - 1];
	}

	template<class Type>
	Type Stack<Type>::Pop()
	{
		return Array<Type>::PopBack();
	}
}