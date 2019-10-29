#pragma once
#include "../Types.h"
#include "../debug/Debug.h"

#include "Array.h"
#include "Stack.h"
#include "Map.h"

namespace Quartz
{
	template<typename Type, UInt32 SIZE>
	class PoolBlock
	{
	private:
		union
		{
			Type mValues[SIZE];
			char mData[SIZE * sizeof(Type)];
		};

	public:
		PoolBlock();

		Type* CreateNoInit(UInt32 index);

		template<typename... Args>
		Type* Create(UInt32 index, Args&&... args);

		void Free(UInt32 index);

		Type& operator[](UInt32 index);
	};

	class IPool
	{
	public:
		virtual ~IPool() {};

		virtual void Free(void* pType) = 0;
	};

	template<typename Type, UInt32 BLOCK_SIZE = 1024U>
	class Pool : public IPool
	{
	public:
		class Iterator
		{
		private:
			typename Map<UInt64, UInt32>::Iterator pItr;

		public:
			Iterator(typename Map<UInt64, UInt32>::Iterator itr);
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

	private:
		Array<PoolBlock<Type, BLOCK_SIZE>*> mBlocks;
		UInt32 mUsed;
		UInt32 mCapacity;

		Map<UInt64, UInt32> mPtrIndexMap;

		Stack<UInt32> mFreeIndices;
		UInt32 mMaxIndex;

		UInt32 NextIndex();

	public:
		Pool();
		Pool(const Pool& pool);
		Pool(Pool&& pool);

		~Pool();

		Type* CreateNoInit();

		template<typename... Args>
		Type* Create(Args&&... args);

		void Free(void* pType) override;

		Iterator begin();
		Iterator end();

		FORCE_INLINE UInt32 GetUsed() { return mUsed; }
		FORCE_INLINE UInt32 GetCapacity() { return mCapacity; }
		FORCE_INLINE UInt32 GetBlockCount() { return mCapacity / BLOCK_SIZE; }
	};

	template<typename Type, UInt32 SIZE>
	PoolBlock<Type, SIZE>::PoolBlock()
		: mData{}
	{
		// Nothing
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Pool<Type, BLOCK_SIZE>::Iterator::Iterator(typename Map<UInt64, UInt32>::Iterator itr)
		: pItr(itr)
	{
		// Nothing
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Pool<Type, BLOCK_SIZE>::Iterator::Iterator(const Iterator& it)
		: pItr(it.pItr)
	{
		// Nothing
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Pool<Type, BLOCK_SIZE>::Iterator::Iterator(Iterator&& it) noexcept
		: pItr(it.pItr)
	{
		// Nothing
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	typename Pool<Type, BLOCK_SIZE>::Iterator& Pool<Type, BLOCK_SIZE>::Iterator::operator++()
	{
		++pItr;
		return *this;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	typename Pool<Type, BLOCK_SIZE>::Iterator Pool<Type, BLOCK_SIZE>::Iterator::operator++(int)
	{
		Pool<Type, BLOCK_SIZE>::Iterator temp(*this);
		++pItr;
		return temp;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Bool8 Pool<Type, BLOCK_SIZE>::Iterator::operator==(const Iterator& it) const
	{
		return pItr == it.pItr;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Bool8 Pool<Type, BLOCK_SIZE>::Iterator::operator!=(const Iterator& it) const
	{
		return pItr != it.pItr;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Bool8 Pool<Type, BLOCK_SIZE>::Iterator::operator>=(const Iterator& it) const
	{
		return pItr >= it.pItr;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Bool8 Pool<Type, BLOCK_SIZE>::Iterator::operator<=(const Iterator& it) const
	{
		return pItr <= it.pItr;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Bool8 Pool<Type, BLOCK_SIZE>::Iterator::operator>(const Iterator& it) const
	{
		return pItr > it.pItr;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Bool8 Pool<Type, BLOCK_SIZE>::Iterator::operator<(const Iterator& it) const
	{
		return pItr < it.pItr;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	typename Pool<Type, BLOCK_SIZE>::Iterator& Pool<Type, BLOCK_SIZE>::Iterator::operator=(const Iterator& it)
	{
		pItr = it.pItr;
		return *this;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Type& Pool<Type, BLOCK_SIZE>::Iterator::operator*()
	{
		return *reinterpret_cast<Type*>(pItr->key);
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Type* Pool<Type, BLOCK_SIZE>::Iterator::operator->()
	{
		return (pItr->key);
	}

	template<typename Type, UInt32 SIZE>
	void PoolBlock<Type, SIZE>::Free(UInt32 index)
	{
		DEBUG_ASSERT(index < SIZE && "SubPool index out of bounds!");

		mValues[index].~Type();
	}

	template<typename Type, UInt32 SIZE>
	Type& PoolBlock<Type, SIZE>::operator[](UInt32 index)
	{
		DEBUG_ASSERT(index < SIZE && "SubPool index out of bounds!");
		return mValues[index];
	}

	template<typename Type, UInt32 SIZE>
	Type* PoolBlock<Type, SIZE>::CreateNoInit(UInt32 index)
	{
		return mValues + index;
	}

	template<typename Type, UInt32 SIZE>
	template<typename ...Args>
	Type* PoolBlock<Type, SIZE>::Create(UInt32 index, Args&&... args)
	{
		DEBUG_ASSERT(index < SIZE && "SubPool index out of bounds!");
		return new (mValues + index) Type(std::forward<Args>(args)...);
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	UInt32 Pool<Type, BLOCK_SIZE>::NextIndex()
	{
		if (!mFreeIndices.IsEmpty())
			return mFreeIndices.Pop();

		return mMaxIndex++;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Pool<Type, BLOCK_SIZE>::Pool()
		: mUsed(0), mCapacity(0)
	{
		// Nothing
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Pool<Type, BLOCK_SIZE>::Pool(const Pool& pool)
		: mBlocks(pool.mBlocks), mUsed(pool.mUsed), mCapacity(pool.mCapacity)
	{
		// Nothing
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Pool<Type, BLOCK_SIZE>::Pool(Pool&& pool)
		: mBlocks(pool.mBlocks), mUsed(pool.mUsed), mCapacity(pool.mCapacity)
	{
		// Nothing
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Pool<Type, BLOCK_SIZE>::~Pool()
	{
		for (const auto& pair : mPtrIndexMap)
		{
			const UInt32 blockIndex = pair.value / BLOCK_SIZE;
			const UInt32 subIndex = pair.value % BLOCK_SIZE;
			mBlocks[blockIndex]->Free(subIndex);
		}
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	void Pool<Type, BLOCK_SIZE>::Free(void* pVoid)
	{
		auto it = mPtrIndexMap.Find(reinterpret_cast<UInt64>(pVoid));

		if (it.IsValid())
		{
			const UInt32 blockIndex = it->value / BLOCK_SIZE;
			const UInt32 subIndex = it->value % BLOCK_SIZE;
			mBlocks[blockIndex]->Free(subIndex);
			mFreeIndices.Push(it->value);
			mPtrIndexMap.Erase(it);

			--mUsed;
		}
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	Type* Pool<Type, BLOCK_SIZE>::CreateNoInit()
	{
		const UInt32 index = NextIndex();

		if (index >= mCapacity)
		{
			mBlocks.PushBack(new PoolBlock<Type, BLOCK_SIZE>());
			mCapacity += BLOCK_SIZE;
		}

		const UInt32 blockIndex = index / BLOCK_SIZE;
		const UInt32 subIndex = index % BLOCK_SIZE;

		PoolBlock<Type, BLOCK_SIZE>* mpBlock = mBlocks[blockIndex];
		Type* pObject = mpBlock->CreateNoInit(subIndex);
		mPtrIndexMap[reinterpret_cast<UInt64>(pObject)] = index;

		++mUsed;

		return pObject;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	template<typename ...Args>
	Type* Pool<Type, BLOCK_SIZE>::Create(Args&&... args)
	{
		const UInt32 index = NextIndex();

		if (index >= mCapacity)
		{
			mBlocks.PushBack(new PoolBlock<Type, BLOCK_SIZE>());
			mCapacity += BLOCK_SIZE;
		}

		const UInt32 blockIndex = index / BLOCK_SIZE;
		const UInt32 subIndex = index % BLOCK_SIZE;

		PoolBlock<Type, BLOCK_SIZE>* pBlock = mBlocks[blockIndex];
		Type* pObject = pBlock->Create(subIndex, std::forward<Args>(args)...);
		mPtrIndexMap[reinterpret_cast<UInt64>(pObject)] = index;

		++mUsed;

		return pObject;
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	typename Pool<Type, BLOCK_SIZE>::Iterator Pool<Type, BLOCK_SIZE>::begin()
	{
		return Iterator(mPtrIndexMap.begin());
	}

	template<typename Type, UInt32 BLOCK_SIZE>
	typename Pool<Type, BLOCK_SIZE>::Iterator Pool<Type, BLOCK_SIZE>::end()
	{
		return Iterator(mPtrIndexMap.end());
	}
}