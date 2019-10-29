#pragma once
#include "../Types.h"

namespace Quartz
{
	template<typename HashType>
	UInt32 Hash(const HashType& value) = delete;

	template<>
	FORCE_INLINE UInt32 Hash<Int32>(const Int32& value)
	{
		return value;
	}

	template<>
	FORCE_INLINE UInt32 Hash<UInt32>(const UInt32& value)
	{
		return value;
	}

	template<>
	FORCE_INLINE UInt32 Hash<UInt64>(const UInt64& value)
	{
		UInt64 a = value;
		a ^= a >> 33;
		a *= 0xff51afd7ed558ccdL;
		a ^= a >> 33;
		a *= 0xc4ceb9fe1a85ec53L;
		a ^= a >> 33;
		return static_cast<UInt32>(a);
	}
}