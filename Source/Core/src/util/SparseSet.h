#pragma once

#include "../Common.h"
#include "Array.h"
#include "Utility.h"

namespace Quartz
{
	template<typename _ValueType = UInt32, typename _IntType = UInt32, USize blockSize = 64>
	class SparseSet
	{
	public:
		constexpr static USize ENTITIES_PER_BLOCK = blockSize;

		using ValueType			= _ValueType;
		using IntType			= _IntType;
		using SparseSetType		= SparseSet<_ValueType, _IntType, blockSize>;
		using Iterator			= typename Array<ValueType>::Iterator;

		using SparseBlock		= Array<IntType>;

		constexpr static IntType NULL_INDEX = IntType(-1);

	private:
		Array<ValueType>	mDense;
		Array<SparseBlock>	mSparse;

	private:
		IntType GetBlockIndex(ValueType handle) const
		{
			return IntType(handle) / ENTITIES_PER_BLOCK;
		}

		IntType GetBlockOffset(ValueType handle) const
		{
			return IntType(handle) & (ENTITIES_PER_BLOCK - 1);
		}

	public:
		SparseSet()
			: mDense(), mSparse() {}

		IntType Insert(ValueType handle)
		{
			const IntType blockIndex = GetBlockIndex(handle);
			const IntType blockOffset = GetBlockOffset(handle);

			if (Contains(handle))
			{
				return mSparse[blockIndex][blockOffset];
			}

			if (blockIndex >= mSparse.Size())
			{
				mSparse.Resize(blockIndex + 1);
			}

			if (mSparse[blockIndex].Size() == 0)
			{
				mSparse[blockIndex].Resize(ENTITIES_PER_BLOCK, NULL_INDEX);
			}

			mDense.PushBack(handle);
			mSparse[blockIndex][blockOffset] = mDense.Size() - 1;

			return mSparse[blockIndex][blockOffset];
		}

		void Remove(ValueType handle)
		{
			assert(Contains(handle) && "SparseSet doesn't contain handle!");

			const IntType blockIndex = GetBlockIndex(handle);
			const IntType blockOffset = GetBlockOffset(handle);

			mDense[mSparse[blockIndex][blockOffset]] = mDense[mDense.Size() - 1];

			const IntType removedBlockIndex = GetBlockIndex(mDense[mDense.Size() - 1]);
			const IntType removedBlockOffset = GetBlockOffset(mDense[mDense.Size() - 1]);

			mSparse[removedBlockIndex][removedBlockOffset] = mSparse[blockIndex][blockOffset];
			mSparse[blockIndex][blockOffset] = NULL_INDEX;

			mDense.PopBack();
		}

		Bool8 Contains(ValueType handle) const
		{
			const IntType blockIndex = GetBlockIndex(handle);
			const IntType blockOffset = GetBlockOffset(handle);

			if (blockIndex >= mSparse.Size() || mSparse[blockIndex].Size() == 0)
			{
				return false;
			}

			return mSparse[blockIndex][blockOffset] != NULL_INDEX;
		}

		IntType Index(ValueType handle) const
		{
			const IntType blockIndex = GetBlockIndex(handle);
			const IntType blockOffset = GetBlockOffset(handle);

			return mSparse[blockIndex][blockOffset];
		}

		Iterator begin()
		{
			return mDense.begin();
		}

		Iterator end()
		{
			return mDense.end();
		}

		USize Size() const
		{
			return mDense.Size();
		}
	};
}