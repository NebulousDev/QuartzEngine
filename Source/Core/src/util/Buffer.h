#pragma once

#include "../Common.h"
#include <cstdlib>
#include <cstring>
#include <new>

#include <cassert>

namespace Quartz
{
	template<typename _Type>
	class Buffer
	{
	public:
		using Type			= _Type;
		using BufferType	= Buffer<Type>;

	private:
		Type* mpData;
		USize mSize;
		USize mCapacity;

	private:
		USize NextSize(USize size)
		{
			return size == 0 ? 16 : (((Float32)size * 1.5f) + 0.5f);
		}

		friend void Swap(BufferType& array1, BufferType& array2)
		{
			using Quartz::Swap;
			Swap(array1.mpData, array2.mpData);
			Swap(array1.mSize, array2.mSize);
			Swap(array1.mCapacity, array2.mCapacity);
		}

	public:
		Buffer()
			: mpData(nullptr), mSize(0), mCapacity(0) {}

		Buffer(USize capacity)
			: mSize(0), mCapacity(capacity)
		{
			mpData = static_cast<Type*>(malloc(mCapacity * sizeof(Type)));

			for (USize i = 0; i < mCapacity; i++)
			{
				// Construct new default-constructed value
				new (&mpData[i]) Type();
			}
		}

		Buffer(const BufferType& buffer)
			: mSize(buffer.mSize), mCapacity(buffer.mCapacity)
		{
			mpData = static_cast<Type*>(malloc(buffer.mCapacity * sizeof(Type)));

			for (USize i = 0; i < buffer.mSize; i++)
			{
				// Construct value from given value from the array
				new (&mpData[i]) Type(buffer.mpData[i]);
			}
		}

		Buffer(BufferType&& buffer) noexcept
			: BufferType()
		{
			Swap(*this, buffer);
		}

		~Buffer()
		{
			for (USize i = 0; i < mSize; i++)
			{
				// Destruct valid entries before freeing
				mpData[i].~Type();
			}

			free(mpData);
		}

		template<typename ValueType>
		USize Push(const ValueType& value)
		{
			USize currentIndex = mSize;
			USize nextIndex = currentIndex + sizeof(ValueType) / sizeof(Type);

			// Pushed type is smaller than storage type
			if (nextIndex == currentIndex)
			{
				++nextIndex;
			}

			if (nextIndex > mCapacity)
			{
				Reserve(NextSize(mCapacity));
			}

			// Construct at new index
			new (mpData + currentIndex) ValueType(value);

			mSize = nextIndex;
			
			return currentIndex;
		}

		template<typename ValueType>
		ValueType& Get(USize index)
		{
			assert(index < mSize);

			return (ValueType&)mpData[index];
		}

		void Reserve(USize capacity)
		{
			Type* mpPrev = mpData;
			mpData = static_cast<Type*>(calloc(capacity, sizeof(Type)));

			for (USize i = 0; i < mSize; i++)
			{
				Swap(mpData[i], mpPrev[i]);
			}

			mCapacity = capacity;

			// No need to destruct values because 
			// all valid entries have been swapped
			free(mpPrev);
		}

		BufferType& operator=(BufferType buffer)
		{
			Swap(*this, buffer);
			return *this;
		}

		Type* Data()
		{
			return mpData;
		}

		USize Size()
		{
			return mSize;
		}
	};

	typedef Buffer<Byte> ByteBuffer;
	typedef Buffer<Int32> IntBuffer;
	typedef Buffer<UInt32> UIntBuffer;
	typedef Buffer<Float32> FloatBuffer;
}
