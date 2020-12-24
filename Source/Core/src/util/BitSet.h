#pragma once

#include "../Common.h"
#include <cassert>

namespace Quartz
{
	template<UInt32 Count>
	class BitSet
	{
	public:
		using BitSetType = BitSet<Count>;

		constexpr static UInt32 BIT_COUNT = Count;
		constexpr static UInt32 BYTE_COUNT = Count < 8 ? 1 : Count / 8;

	private:
		Byte mData[BYTE_COUNT];

	public:
		BitSet() : mData{} {};

		void Set(UInt32 bit)
		{
			assert(bit < BIT_COUNT);
			mData[bit / 8] |= (0b1 << (bit & 7));
		}

		void Reset(UInt32 bit)
		{
			assert(bit < BIT_COUNT);
			mData[bit / 8] &= ~(0b1 << (bit & 7));
		}

		Bool8 Get(UInt32 bit)
		{
			assert(bit < BIT_COUNT);
			return mData[bit / 8] & (0b1 << (bit & 7));
		}

		Bool8 operator==(const BitSetType& bitSet)
		{
			Byte diff;

			// Should get unrolled
			for (UInt32 i = 0; i < BYTE_COUNT; ++i)
			{
				diff = mData[i] & ~bitSet.mData[i];
			}

			return diff;
		}

		Bool8 operator!=(const BitSetType& bitSet)
		{
			return !operator==(bitset);
		}
	};
}