#pragma once

#include "../Common.h"
#include <cstdlib>
#include <cstring>
#include <new>

#include "../debug/Debug.h"

namespace Quartz
{
	template<typename _ValueType>
	class Array
	{
	public:
		using ValueType = _ValueType;
		using SizeType	= USize;
		using ArrayType = Array<_ValueType>;

		class Iterator
		{
		private:
			ValueType* pItr;

		public:
			Iterator()
				: pItr(nullptr) { }

			Iterator(ValueType* pItr)
				: pItr(pItr) { }

			Iterator(const Iterator& it)
				: pItr(it.pItr) { }

			Iterator& operator++()
			{
				++pItr;
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator temp(*this);
				++pItr;
				return *temp;
			}

			Bool8 operator==(const Iterator& it) const
			{
				return pItr == it.pItr;
			}

			Bool8 operator!=(const Iterator& it) const
			{
				return pItr != it.pItr;
			}

			Bool8 operator>=(const Iterator& it) const
			{
				return pItr >= it.pItr;
			}

			Bool8 operator<=(const Iterator& it) const
			{
				return pItr <= it.pItr;
			}

			Bool8 operator>(const Iterator& it) const
			{
				return pItr > it.pItr;
			}

			Bool8 operator<(const Iterator& it) const
			{
				return pItr < it.pItr;
			}

			ValueType& operator*()
			{
				return *pItr;
			}

			ValueType* operator->()
			{
				return pItr;
			}
		};

	protected:
		ValueType*	mpData;
		SizeType	mSize;
		SizeType	mCapacity;

	protected:
		SizeType NextSize(SizeType size)
		{
			return size == 0 ? 16 : static_cast<SizeType>((static_cast<Float32>(size) * 1.5f) + 0.5f);
		}

		friend void Swap(ArrayType& array1, ArrayType& array2)
		{
			using Quartz::Swap;
			Swap(array1.mpData, array2.mpData);
			Swap(array1.mSize, array2.mSize);
			Swap(array1.mCapacity, array2.mCapacity);
		}

		void ReserveImpl(SizeType capacity, SizeType offset = 0)
		{
			ValueType* mpPrev = mpData;
			mpData = static_cast<ValueType*>(malloc(capacity * sizeof(ValueType)));

			for (SizeType i = 0; i < mSize; i++)
			{
				Swap(mpData[i + offset], mpPrev[i]);
			}

			mCapacity = capacity;

			// No need to destruct values because 
			// all valid entries have been swapped
			free(mpPrev);
		}

	public:
		Array()
			: mpData(nullptr), mSize(0), mCapacity(0) {}

		Array(SizeType size)
			: mSize(size), mCapacity(size)
		{
			mpData = static_cast<ValueType*>(malloc(size * sizeof(ValueType)));

			for (SizeType i = 0; i < mSize; i++)
			{
				// Construct new default-constructed value
				new (&mpData[i]) ValueType();
			}
		}

		Array(SizeType size, const ValueType& value)
			: mSize(size), mCapacity(size)
		{
			mpData = static_cast<ValueType*>(malloc(size * sizeof(ValueType)));

			for (SizeType i = 0; i < mSize; i++)
			{
				// Construct new value with given initial value
				new (&mpData[i]) ValueType(value);
			}
		}

		Array(const ArrayType& array)
			: mSize(array.mSize), mCapacity(array.mCapacity)
		{
			mpData = static_cast<ValueType*>(malloc(array.mCapacity * sizeof(ValueType)));

			for (SizeType i = 0; i < mSize; i++)
			{
				// Construct value from given value from the array
				new (&mpData[i]) ValueType(array.mpData[i]);
			}
		}

		Array(ArrayType&& array) noexcept :
			ArrayType()
		{
			Swap(*this, array);
		}

		~Array()
		{
			Clear();
			free(mpData);
		}

		ValueType* PushFront(const ValueType& value)
		{
			if (mSize + 1 > mCapacity)
			{
				// Reserve with offset of 1
				// so that we dont need to move
				// data again after resizing
				ReserveImpl(NextSize(mCapacity), 1);
			}
			else
			{
				// Move all values right by one
				memmove(&mpData[1], &mpData[0], mSize * sizeof(ValueType));
			}

			++mSize;

			// Construct at the beginning of the array
			return new (&mpData[0]) ValueType(value);

		}

		ValueType* PushFront(ValueType&& rValue)
		{
			if (mSize + 1 > mCapacity)
			{
				// Reserve with offset of 1
				// so that we dont need to move
				// data again after resizing
				ReserveImpl(NextSize(mCapacity), 1);
			}
			else
			{
				// Move all values right by one
				memmove(&mpData[1], &mpData[0], mSize * sizeof(ValueType));
			}

			++mSize;

			// Construct at the beginning of the array
			return new (&mpData[0]) ValueType(Move(rValue));
		}

		ValueType* PushBack(const ValueType& value)
		{
			if (mSize + 1 > mCapacity)
			{
				ReserveImpl(NextSize(mCapacity));
			}

			// Construct at the end of the array
			return new (&mpData[mSize++]) ValueType(value);
		}

		ValueType* PushBack(ValueType&& rValue)
		{
			if (mSize + 1 > mCapacity)
			{
				ReserveImpl(NextSize(mCapacity));
			}

			// Construct at the end of the array
			return new (&mpData[mSize++]) ValueType(Move(rValue));
		}

		void PopBack()
		{
			if (mSize > 0)
			{
				Remove(mSize - 1);
			}
		}

		void PopFront()
		{
			if (mSize > 0)
			{
				Remove(0);
			}
		}

		void Remove(SizeType index)
		{
			mpData[index].~ValueType();

			if (mSize > 0 && index < mSize)
			{
				// Move all values left by one
				memmove(&mpData[index], &mpData[index + 1], (mSize - index) * sizeof(ValueType));
				--mSize;
			}
		}

		Bool8 Contains(const ValueType& value)
		{
			for (const ValueType& match : *this)
			{
				if (value == match)
				{
					return true;
				}
			}
			
			return false;
		}

		void Resize(SizeType size)
		{
			if (size < mSize)
			{
				// Cannot resize smaller than mSize
				return;
			}

			if (size > mCapacity)
			{
				// Size is larger than capacity, reserve more space
				ReserveImpl(size);
			}

			for (SizeType i = mSize; i < size; i++)
			{
				// Construct new default-constructed value
				new (&mpData[i]) ValueType();
			}

			mSize = size;
		}

		void Resize(SizeType size, const ValueType& value)
		{
			if (size > mSize)
			{
				if (size > mCapacity)
				{
					Reserve(NextSize(size));
				}
				
				for (SizeType i = mSize; i < size; i++)
				{
					// Construct new value with given initial value
					new (&mpData[i]) ValueType(value);
				}
			}

			else if (size < mSize)
			{
				for (SizeType i = mSize; i >= size; --i)
				{
					// Destruct valid entries
					mpData[i].~ValueType();
				}
			}

			mSize = size;
		}

		void Reserve(SizeType capacity)
		{
			if (capacity < mSize)
			{
				// Cannot resize smaller than mSize
				return;
			}

			ReserveImpl(capacity);
		}

		void Shrink()
		{
			ReserveImpl(mSize);
		}

		void Clear()
		{
			for (SizeType i = 0; i < mSize; i++)
			{
				// Destruct valid entries
				mpData[i].~ValueType();
			}

			mSize = 0;
		}

		ArrayType& operator=(ArrayType array)
		{
			Swap(*this, array);
			return *this;
		}

		ValueType& operator[](SizeType index)
		{
			DEBUG_ASSERT(index < mSize && "Array index out of bounds!");
			return mpData[index];
		}

		const ValueType& operator[](SizeType index) const
		{
			DEBUG_ASSERT(index < mSize && "Array index out of bounds!");
			return mpData[index];
		}

		Iterator begin()
		{
			if (!mpData)
			{
				return nullptr;
			}

			return Iterator(mpData);
		}

		Iterator end()
		{
			if (!mpData)
			{
				return nullptr;
			}

			return Iterator(mpData + mSize);
		}

		////////////////////////////////

		Iterator begin() const
		{
			if (!mpData)
			{
				return nullptr;
			}

			return Iterator(mpData);
		}

		Iterator end() const
		{
			if (!mpData)
			{
				return nullptr;
			}

			return Iterator(mpData + mSize);
		}

		////////////////////////////////

		ValueType* Data()
		{
			return mpData;
		}

		const ValueType* Data() const
		{
			return mpData;
		}

		SizeType Size() const
		{
			return mSize;
		}

		SizeType Capacity() const
		{
			return mCapacity;
		}

		Bool8 IsEmpty() const
		{
			return mSize == 0;
		}
	};
}