#pragma once
#include "../Common.h"
#include "../debug/Debug.h"

#include "Utils.h"
#include "../memory/Memory.h"

#include <utility>
#include <cassert>

#include <initializer_list>

namespace Quartz
{
	template<class Type>
	class Array
	{
	public:
		class Iterator
		{
		private:
			Type* pItr;

		public:
			Iterator(Type* pItr);
			Iterator(const Iterator& it);
			Iterator(Iterator&& it) noexcept;

			Iterator& operator++();
			Iterator operator++(int);

			Bool8 operator==(const Iterator& it) const;
			Bool8 operator!=(const Iterator& it) const;
			Bool8 operator>=(const Iterator& it) const;
			Bool8 operator<=(const Iterator& it) const;
			Bool8 operator>(const Iterator& it) const;
			Bool8 operator<(const Iterator& it) const;

			Iterator& operator=(const Iterator& it);

			Type& operator*();
			Type* operator->();
		};

	protected:
		Type* mpData;
		UInt32 mSize;
		UInt32 mCapacity;

	private:
		void Swap(Array& array) noexcept;

	public:
		Array();
		explicit Array(UInt32 initSize);
		Array(UInt32 initSize, const Type& value);
		Array(UInt32 initSize, Type&& value);
		Array(const Array& array);
		Array(Array&& array) noexcept;

		Array(const std::initializer_list<Type>& list);

		~Array();

		void Resize(UInt32 size);
		void Resize(UInt32 size, const Type& value);
		void Resize(UInt32 size, Type&& value);

		void Extend(UInt32 size = 1);
		void Extend(UInt32 size, const Type& value);
		void Extend(UInt32 size, Type&& value);
		void Extend(UInt32 index, UInt32 size);
		void Extend(UInt32 index, UInt32 size, const Type& value);
		void Extend(UInt32 index, UInt32 size, Type&& value);

		void Shrink(UInt32 size = 1);
		void Shrink(UInt32 index, UInt32 size);

		void ExtendToFit();
		void ShrinkToFit();

		void Reserve(UInt32 capacity);

		void PushFront(const Type& value);
		void PushFront(Type&& value);

		void PushBack(const Type& value);
		void PushBack(Type&& value);

		Type PopFront();
		Type PopBack();

		void Insert(UInt32 index, const Type& value);
		void Insert(UInt32 index, Type&& value);

		void Erase(UInt32 index);
		
		void Clear();

		Type& Get(UInt32 index);

		Type& operator[](UInt32 index);
		Type operator[](UInt32 index) const;

		Array& operator=(const Array& array);
		Array& operator=(Array&& array) noexcept;

		Iterator begin();
		Iterator end();

		FORCE_INLINE Bool8 IsEmpty() { return mSize == 0; }

		FORCE_INLINE Type* Data() { return mpData; }
		FORCE_INLINE UInt32 Size() const { return mSize; }
		FORCE_INLINE UInt32 GetCapacity() const { return mCapacity; }
	};

	template<class Type>
	Array<Type>::Iterator::Iterator(Type* pItr)
		: pItr(pItr)
	{
		// Nothing
	}

	template<class Type>
	Array<Type>::Iterator::Iterator(const Iterator& it)
		: pItr(it.pItr)
	{
		// Nothing
	}

	template<class Type>
	Array<Type>::Iterator::Iterator(Iterator&& it) noexcept
		: pItr(it.pItr)
	{
		// Nothing
	}

	template<class Type>
	typename Array<Type>::Iterator& Array<Type>::Iterator::operator++()
	{
		++pItr;
		return *this;
	}

	template<class Type>
	typename Array<Type>::Iterator Array<Type>::Iterator::operator++(int)
	{
		Array<Type>::Iterator temp(*this);
		++pItr;
		return temp;
	}

	template<class Type>
	Bool8 Array<Type>::Iterator::operator==(const Iterator& it) const
	{
		return pItr == it.pItr;
	}

	template<class Type>
	Bool8 Array<Type>::Iterator::operator!=(const Iterator& it) const
	{
		return pItr != it.pItr;
	}

	template<class Type>
	Bool8 Array<Type>::Iterator::operator>=(const Iterator& it) const
	{
		return pItr >= it.pItr;
	}

	template<class Type>
	Bool8 Array<Type>::Iterator::operator<=(const Iterator& it) const
	{
		return pItr <= it.pItr;
	}

	template<class Type>
	Bool8 Array<Type>::Iterator::operator>(const Iterator& it) const
	{
		return pItr > it.pItr;
	}

	template<class Type>
	Bool8 Array<Type>::Iterator::operator<(const Iterator& it) const
	{
		return pItr < it.pItr;
	}

	template<class Type>
	typename Array<Type>::Iterator& Array<Type>::Iterator::operator=(const Iterator& it)
	{
		pItr = it.pItr;
		return *this;
	}

	template<class Type>
	Type& Array<Type>::Iterator::operator*()
	{
		return *pItr;
	}

	template<class Type>
	Type* Array<Type>::Iterator::operator->()
	{
		return pItr;
	}

	template<class Type>
	void Array<Type>::Swap(Array& array) noexcept
	{
		std::swap(mpData, array.mpData);
		std::swap(mSize, array.mSize);
		std::swap(mCapacity, array.mCapacity);
	}

	template<class Type>
	Array<Type>::Array()
		: mpData(nullptr), mSize(0), mCapacity(0)
	{
		// Nothing
	}

	template<class Type>
	Array<Type>::Array(UInt32 initSize)
		: mpData(nullptr), mSize(0), mCapacity(0)
	{
		Resize(initSize);
	}

	template<class Type>
	Array<Type>::Array(UInt32 initSize, const Type& value)
		: mpData(nullptr), mSize(0), mCapacity(0)
	{
		Resize(initSize, std::forward<Type>(value));
	}

	template<class Type>
	Array<Type>::Array(UInt32 initSize, Type&& value)
		: mpData(nullptr), mSize(0), mCapacity(0)
	{
		Resize(initSize, std::forward<Type>(value));
	}

	template<class Type>
	Array<Type>::Array(const Array& array)
		: mSize(array.mSize), mCapacity(array.mCapacity)
	{
		mpData = static_cast<Type*>(malloc(mCapacity * sizeof(Type)));

		Type* pOther = array.mpData;
		for (Type* pValue = mpData; pValue < mpData + array.mSize; ++pValue, ++pOther)
			new (pValue) Type(*pOther);
	}

	template<class Type>
	Array<Type>::Array(Array&& array) noexcept
		: mpData(array.mpData), mSize(array.mSize), mCapacity(array.mCapacity)
	{
		array.mpData = nullptr;

		// Are these necessary?
		array.mSize = 0;
		array.mCapacity = 0;
	}

	template<class Type>
	Array<Type>::Array(const std::initializer_list<Type>& list)
		: Array(list.size())
	{
		UInt32 i = 0;
		for (const Type* pVal = list.begin(); pVal != list.end(); ++pVal, ++i)
			new (&mpData[i]) Type(*pVal);
	}

	template<class Type>
	Array<Type>::~Array()
	{
		if (mpData)
		{
			// Manually destruct destroyed values
			for (Type* pValue = mpData;
				pValue != mpData + mSize; ++pValue)
			{
				pValue->~Type();
			}

			free(mpData);
		}
	}

	template<class Type>
	void Array<Type>::Resize(UInt32 size)
	{
		if (size <= mCapacity)
		{
			mSize = size;
			return;
		}

		Reserve(NextPowerOf2(size));
		mSize = size;
	}

	template<class Type>
	void Array<Type>::Resize(UInt32 size, const Type& value)
	{
		if (size <= mCapacity)
		{
			mSize = size;
			return;
		}

		Reserve(NextPowerOf2(size));

		for (Type* pValue = mpData + mSize;
			pValue != mpData + mCapacity; ++pValue)
		{
			new (pValue) Type(value);
		}

		mSize = size;
	}

	template<class Type>
	void Array<Type>::Resize(UInt32 size, Type&& value)
	{
		if (size <= mCapacity)
		{
			mSize = size;
			return;
		}

		Reserve(NextPowerOf2(size));

		for (Type* pValue = mpData + mSize; 
			pValue != mpData + mCapacity; ++pValue)
		{
			new (pValue) Type(value);
		}

		mSize = size;
	}

	template<class Type>
	void Array<Type>::Extend(UInt32 size)
	{
		mSize += size;
		if (mSize > mCapacity)
			Reserve(NextPowerOf2(mSize));
	}

	template<class Type>
	void Array<Type>::Extend(UInt32 size, const Type& value)
	{
		Resize(mSize + size, value);
	}

	template<class Type>
	void Array<Type>::Extend(UInt32 size, Type&& value)
	{
		Resize(mSize + size, value);
	}

	template<class Type>
	void Array<Type>::Extend(UInt32 index, UInt32 size)
	{
		DEBUG_ASSERT(index < mSize && "Array index out of bounds!");

		Extend(size);

		MemMove(mpData + index, mpData + index + size, mSize * sizeof(Type));
	}

	template<class Type>
	void Array<Type>::Extend(UInt32 index, UInt32 size, const Type& value)
	{
		Extend(index, size);

		for (Type* pValue = mpData + index;
			pValue != mpData + index + size; ++pValue)
		{
			*pValue = value;
		}
	}

	template<class Type>
	void Array<Type>::Extend(UInt32 index, UInt32 size, Type&& value)
	{
		Extend(index, size);

		for (Type* pValue = mpData + index;
			pValue != mpData + index + size; ++pValue)
		{
			*pValue = value;
		}
	}

	template<class Type>
	void Array<Type>::Shrink(UInt32 size)
	{
		mSize -= size;

		// Manually destruct destroyed values
		for (Type* pValue = mpData + mSize;
			pValue != mpData + mSize + size; ++pValue)
		{
			pValue->~Type();
		}
	}

	template<class Type>
	void Array<Type>::Shrink(UInt32 index, UInt32 size)
	{
		const UInt32 remaining = mSize - index - size;

		DEBUG_ASSERT(index < mSize && "Array index out of bounds!");
		DEBUG_ASSERT(size < remaining && "Array size out of bounds!");

		MemMove(mpData + index + size, mpData + index, remaining * sizeof(Type));

		mSize -= size;

		// Manually destruct destroyed values
		for (Type* pValue = mpData + mSize;
			pValue != mpData + mSize + size; ++pValue)
		{
			pValue->~Type();
		}
	}

	template<class Type>
	void Array<Type>::ExtendToFit()
	{
		Resize(mCapacity);
	}

	template<class Type>
	void Array<Type>::ShrinkToFit()
	{
		Reserve(mSize);
	}

	template<class Type>
	void Array<Type>::Reserve(UInt32 capacity)
	{
		const UInt32 newSize = capacity > mSize ? mSize : capacity;

		Type* pNewData = capacity ? static_cast<Type*>(malloc(capacity * sizeof(Type))) : nullptr;
		//Type* pNewData = capacity ? static_cast<Type*>(calloc(capacity, sizeof(Type))) : nullptr;

		if (mpData)
		{
			MemCopy(mpData, pNewData, newSize * sizeof(Type));

			if (newSize < mSize)
			{
				for (Type* pValue = mpData + newSize;
					pValue != mpData + newSize + mSize - 1; ++pValue)
				{
					pValue->~Type();
				}
			}

			free(mpData);
		}

		mpData = pNewData;
		mSize = newSize;
		mCapacity = capacity;
	}

	template<class Type>
	void Array<Type>::PushFront(const Type& value)
	{
		Extend(0, 1, value);
	}

	template<class Type>
	void Array<Type>::PushFront(Type&& value)
	{
		Extend(0, 1, value);
	}

	template<class Type>
	void Array<Type>::PushBack(const Type& value)
	{
		Extend();
		new (mpData + mSize - 1) Type(value);
	}

	template<class Type>
	void Array<Type>::PushBack(Type&& value)
	{
		Extend();
		new (mpData + mSize - 1) Type(std::forward<Type>(value));
	}

	template<class Type>
	Type Array<Type>::PopFront()
	{
		DEBUG_ASSERT(mSize > 0 && "Array size out of bounds!");

		Type value = mpData[0];
		Shrink(0, 1);

		return value;
	}

	template<class Type>
	Type Array<Type>::PopBack()
	{
		DEBUG_ASSERT(mSize > 0 && "Array size out of bounds!");
		return mpData[--mSize];
	}

	template<class Type>
	void Array<Type>::Insert(UInt32 index, const Type& value)
	{
		Extend(index, 1, value);
	}

	template<class Type>
	void Array<Type>::Insert(UInt32 index, Type&& value)
	{
		Extend(index, 1, value);
	}

	template<class Type>
	void Array<Type>::Erase(UInt32 index)
	{
		Shrink(index, 1);
	}

	template<class Type>
	void Array<Type>::Clear()
	{
		Resize(0);
	}

	template<class Type>
	Type& Array<Type>::Get(UInt32 index)
	{
		DEBUG_ASSERT(index < mSize && "Array index out of bounds!");
		return mpData[index];
	}

	template<class Type>
	Type& Array<Type>::operator[](UInt32 index)
	{
		DEBUG_ASSERT(index < mSize && "Array index out of bounds!");
		return mpData[index];
	}

	template<class Type>
	Type Array<Type>::operator[](UInt32 index) const
	{
		DEBUG_ASSERT(index < mSize && "Array index out of bounds!");
		return mpData[index];
	}

	template<class Type>
	Array<Type>& Array<Type>::operator=(const Array& array)
	{
		Array<Type>(array).Swap(*this);
		return *this;
	}

	template<class Type>
	Array<Type>& Array<Type>::operator=(Array&& array) noexcept
	{
		Array<Type>(std::move(array)).Swap(*this);
		return *this;
	}

	template<class Type>
	typename Array<Type>::Iterator Array<Type>::begin()
	{
		return Iterator(mpData);
	}

	template<class Type>
	typename Array<Type>::Iterator Array<Type>::end()
	{
		return Iterator(mpData + mSize);
	}
}