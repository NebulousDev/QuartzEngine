#pragma once
#include "../Types.h"

#if _MSC_VER

#include <intrin.h>

#if defined(WIN64)
#pragma intrinsic(_BitScanReverse64)
#else
#pragma intrinsic(_BitScanReverse)
#endif

#endif

namespace Quartz
{
	static FORCE_INLINE Bool8 IsPowerOf2(const UInt64 value)
	{
		return !(value & (value - 1));
	}

#if _MSC_VER

#if defined(WIN64)
	static FORCE_INLINE UInt64 NextPowerOf2(const UInt64 value)
	{
		unsigned long idx;
		_BitScanReverse64(&idx, (unsigned long)value - 1);
		return (UInt64)(1U << (idx + 1));
	}

#else
	static FORCE_INLINE UInt32 NextPowerOf2(const UInt32 value)
	{
		/*
		unsigned long idx;
		_BitScanReverse(&idx, (unsigned long)value - 1);
		return (UInt32)(1U << (idx + 1));
		*/

		unsigned int p = 1;
		if (value && !(value & (value - 1)))
			return value;

		while (p < value)
			p <<= 1;

		return p;
	}
#endif

#endif

#if _MSC_VER

#if defined(WIN64)
	static FORCE_INLINE UInt64 NextGreaterPowerOf2(const UInt64 value)
	{
		unsigned long idx;
		_BitScanReverse64(&idx, (unsigned long)value);
		return (UInt64)(1U << (idx + 1));
	}

#else
	static FORCE_INLINE UInt32 NextGreaterPowerOf2(const UInt32 value)
	{
		unsigned long idx;
		_BitScanReverse(&idx, (unsigned long)value);
		return (UInt32)(1U << (idx + 1));
	}
#endif

#endif

	static FORCE_INLINE UInt32 CountZeroBitsRight(const UInt32 value)
	{
		Float32 f = (float)((Int32)value & -(Int32)value);
		return (*(UInt32 *)&f >> 23) - 0x7f;
	}

	template<class Type>
	static FORCE_INLINE void PutBits(char* buff, Type x)
	{
		int buffIdx = 0;
		for (int i = sizeof(x) << 3; i; i--)
			buff[buffIdx++] = ('0' + ((x >> (i - 1)) & 1));
	}

	static FORCE_INLINE UInt32 GetPowerLevel(const UInt32 value)
	{
		UInt32 mask = value & ~(value - 1);
		return CountZeroBitsRight(mask);
	}
}
