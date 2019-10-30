#pragma once
#include "../Common.h"
#include "../memory/Memory.h"

#include <intrin.h>

#include <cmath>

//#define NO_INTRINSICS

#ifdef _MSC_VER
#define VECTOR_CALL __vectorcall
#else
#define VECTOR_CALL __fastcall
#endif

#if defined(__AVX2__) && !defined(NO_INTRINSICS)
#define INTRIN_AVX2
#endif

#if defined(__AVX__) && !defined(NO_INTRINSICS)
#define INTRIN_AVX
#endif

#define XMMVEC_ALIGN __declspec(align(16))

#ifndef USE_FAST_SQRT_2ND_PASS
#define USE_FAST_SQRT_2ND_PASS 0
#endif

namespace Quartz
{
	typedef __m128 XMMVEC;
	typedef __m128i XMMVECi;

	//////////////////////////////////////////////////////////////
	// XMM Float
	//////////////////////////////////////////////////////////////

	struct XMMFloat4
	{
		Float32 x;
		Float32 y;
		Float32 z;
		Float32 w;
	};

	XMMVEC_ALIGN struct XMMFloat4A
	{
		Float32 x;
		Float32 y;
		Float32 z;
		Float32 w;
	};

	struct XMMFloat3
	{
		Float32 x;
		Float32 y;
		Float32 z;
	};

	XMMVEC_ALIGN struct XMMFloat3A
	{
		Float32 x;
		Float32 y;
		Float32 z;
	};

	struct XMMFloat2
	{
		Float32 x;
		Float32 y;
	};

	XMMVEC_ALIGN struct XMMFloat2A
	{
		Float32 x;
		Float32 y;
	};

	//////////////////////////////////////////////////////////////
	// XMM Signed Int
	//////////////////////////////////////////////////////////////

	struct XMMInt4
	{
		Int32 x;
		Int32 y;
		Int32 z;
		Int32 w;
	};

	XMMVEC_ALIGN struct XMMInt4A
	{
		Int32 x;
		Int32 y;
		Int32 z;
		Int32 w;
	};

	struct XMMInt3
	{
		Int32 x;
		Int32 y;
		Int32 z;
	};

	XMMVEC_ALIGN struct XMMInt3A
	{
		Int32 x;
		Int32 y;
		Int32 z;
	};

	struct XMMInt2
	{
		Int32 x;
		Int32 y;
	};

	XMMVEC_ALIGN struct XMMInt2A
	{
		Int32 x;
		Int32 y;
	};

	//////////////////////////////////////////////////////////////
	// XMM Shuffle
	//////////////////////////////////////////////////////////////

	template<UInt8 x, UInt8 y, UInt8 z, UInt8 w>
	FORCE_INLINE XMMVEC VECTOR_CALL XMMShuffleFloat4(const XMMVEC vec)
	{
		return _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(w, z, y, x));
	}

	template<UInt8 x, UInt8 y, UInt8 z>
	FORCE_INLINE XMMVEC VECTOR_CALL XMMShuffleFloat3(const XMMVEC vec)
	{
		return _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(z, z, y, x));
	}

	template<UInt8 x, UInt8 y>
	FORCE_INLINE XMMVEC VECTOR_CALL XMMShuffleFloat2(const XMMVEC vec)
	{
		return _mm_shuffle_ps(vec, vec, _MM_SHUFFLE(y, y, y, x));
	}

	//////////////////////////////////////////////////////////////
	// XMM Misc
	//////////////////////////////////////////////////////////////

	FORCE_INLINE Float32 VECTOR_CALL XMMMinFloat4(const XMMVEC vec)
	{
		return 0;
	}

	FORCE_INLINE Float32 VECTOR_CALL XMMMaxFloat4(const XMMVEC vec)
	{
		return 0;
	}

	//////////////////////////////////////////////////////////////
	// XMM Load/Store Operations
	//////////////////////////////////////////////////////////////

	FORCE_INLINE Float32 VECTOR_CALL XMMGetFloat(const XMMVEC vec, const UInt8&& index)
	{
		return vec.m128_f32[index];
	}

	// -----------------------------------------------------------

	FORCE_INLINE XMMVEC VECTOR_CALL XMMFillFloat4(const Float32 val)
	{
		return _mm_set_ps1(val);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMLoadFloat4(const Float32* pSrc)
	{
		return _mm_loadu_ps(pSrc);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMLoadFloat4A(const Float32* pSrc)
	{
		return _mm_load_ps(pSrc);
	}

	FORCE_INLINE void VECTOR_CALL XMMStoreFloat4(Float32* pSrc, const XMMVEC vec)
	{
		_mm_storeu_ps(pSrc, vec);
	}

	FORCE_INLINE void VECTOR_CALL XMMStoreFloat4A(Float32* pSrc, const XMMVEC vec)
	{
		_mm_store_ps(pSrc, vec);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMSetFloat4(const Float32 x, const Float32 y, const Float32 z, const Float32 w)
	{
		return _mm_set_ps(x, y, z, w);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMSetFloat3(const Float32 x, const Float32 y, const Float32 z)
	{
		return _mm_set_ps(x, y, z, 0.0f);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMSetFloat2(const Float32 x, const Float32 y)
	{
		return _mm_set_ps(x, y, 0.0f, 0.0f);
	}

	//////////////////////////////////////////////////////////////
	// XMM Scalar Operations
	//////////////////////////////////////////////////////////////

	FORCE_INLINE XMMVEC VECTOR_CALL XMMSetScalar(const Float32 scalar)
	{
		return _mm_set_ss(scalar);
	}

	// -----------------------------------------------------------

	FORCE_INLINE XMMVEC VECTOR_CALL XMMAddFloatScalar(const XMMVEC vec1, const XMMVEC vec2)
	{
		return _mm_add_ss(vec1, vec2);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMSubFloatScalar(const XMMVEC vec1, const XMMVEC vec2)
	{
		return _mm_sub_ss(vec1, vec2);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMMulFloatScalar(const XMMVEC vec1, const XMMVEC vec2)
	{
		return _mm_mul_ss(vec1, vec2);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMDivFloatScalar(const XMMVEC vec1, const XMMVEC vec2)
	{
		return _mm_div_ss(vec1, vec2);
	}

	//////////////////////////////////////////////////////////////
	// XMM Vector Operations
	//////////////////////////////////////////////////////////////

	FORCE_INLINE XMMVEC VECTOR_CALL XMMAddFloat4(const XMMVEC vec1, const XMMVEC vec2)
	{
		return _mm_add_ps(vec1, vec2);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMSubFloat4(const XMMVEC vec1, const XMMVEC vec2)
	{
		return _mm_sub_ps(vec1, vec2);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMMulFloat4(const XMMVEC vec1, const XMMVEC vec2)
	{
		return _mm_mul_ps(vec1, vec2);
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMDivFloat4(const XMMVEC vec1, const XMMVEC vec2)
	{
		return _mm_div_ps(vec1, vec2);
	}

	// -----------------------------------------------------------

	FORCE_INLINE XMMVEC VECTOR_CALL XMMHorizAddFloat4(const XMMVEC vec)
	{
		XMMVEC shuffle = _mm_movehdup_ps(vec);
		XMMVEC sums = _mm_add_ps(vec, shuffle);
		shuffle = _mm_movehl_ps(shuffle, sums);
		sums = _mm_add_ss(sums, shuffle);
		return sums;
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMHorizSubFloat4(const XMMVEC vec)
	{
		XMMVEC shuffle = _mm_movehdup_ps(vec);
		XMMVEC diffs = _mm_sub_ps(vec, shuffle);
		shuffle = _mm_movehl_ps(shuffle, diffs);
		diffs = _mm_sub_ss(diffs, shuffle);
		return diffs;
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMHorizMulFloat4(const XMMVEC vec)
	{
		XMMVEC shuffle = _mm_movehdup_ps(vec);
		XMMVEC prods = _mm_mul_ps(vec, shuffle);
		shuffle = _mm_movehl_ps(shuffle, prods);
		prods = _mm_mul_ss(prods, shuffle);
		return prods;
	}

	FORCE_INLINE XMMVEC VECTOR_CALL XMMHorizDivFloat4(const XMMVEC vec)
	{
		XMMVEC shuffle = _mm_movehdup_ps(vec);
		XMMVEC quots = _mm_div_ps(vec, shuffle);
		shuffle = _mm_movehl_ps(shuffle, quots);
		quots = _mm_div_ss(quots, shuffle);
		return quots;
	}

	// -----------------------------------------------------------

	FORCE_INLINE XMMVEC VECTOR_CALL XMMDotFloat4(const XMMVEC vec1, const XMMVEC vec2)
	{
		return _mm_dp_ps(vec1, vec2, 0xFF);
	}

	// -----------------------------------------------------------

	/*
	FORCE_INLINE XMMVEC VECTOR_CALL XMMCrossFloat4(const XMMVEC vec1, const XMMVEC vec2)
	{
		return 0;
	}
	*/

	// -----------------------------------------------------------

	//////////////////////////////////////////////////////////////
	// XMMi Operations
	//////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////
	// XMM String Operations
	//////////////////////////////////////////////////////////////

	// TODO

	//////////////////////////////////////////////////////////////
	// Fast Math
	//////////////////////////////////////////////////////////////

	// https://en.wikipedia.org/wiki/Fast_inverse_square_root
	FORCE_INLINE Float32 FastInvsereSquare(const Float32& number)
	{
		long i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y));
#if USE_FAST_SQRT_2ND_PASS
		y = y * (threehalfs - (x2 * y * y));
#endif
		return y;
	}

	// https://stackoverflow.com/questions/11644441/fast-inverse-square-root-on-x64
	FORCE_INLINE Double64 FastInvsereSquare64(const Double64& number)
	{
		Double64 y = number;
		Double64 x2 = y * 0.5;
		UInt64 i = *(UInt64*)&y;
		i = 0x5fe6eb50c7b537a9 - (i >> 1);
		y = *(Double64*)&i;
		y = y * (1.5 - (x2 * y * y));
#if USE_FAST_SQRT_2ND_PASS
		y = y * (1.5 - (x2 * y * y));
#endif
		return y;
	}

}