#pragma once

//////////////////////////////////////////////////////////////
// Preprocessor Definitions
//////////////////////////////////////////////////////////////

#include "Defines.h"
#include <cmath>

#define PIE					3.14159265f
#define INFINITY			static_cast<float>(0x7F800000)
#define NEGATIVE_INFINITY	static_cast<float>(0xFF800000)
#define NAN					static_cast<float>(0x7FBFFFFF)

#define USE_FAST_SQRT_2ND_PASS 0

/** Fast inverse square root 
	https://en.wikipedia.org/wiki/Fast_inverse_square_root
*/
FORCEINLINE float FastInvsereSquare(float number)
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

/** Fast inverse square root (64bit)
	https://stackoverflow.com/questions/11644441/fast-inverse-square-root-on-x64
*/
FORCEINLINE double FastInvsereSquare64(double number)
{
	double y = number;
	double x2 = y * 0.5;
	__int64 i = *(__int64*)&y;
	i = 0x5fe6eb50c7b537a9 - (i >> 1);
	y = *(double*)&i;
	y = y * (1.5 - (x2 * y * y));

	#if USE_FAST_SQRT_2ND_PASS
		y = y * (1.5 - (x2 * y * y));
	#endif

	return y;
}

FORCEINLINE float ToRadians(float degrees)
{
	return degrees * (PIE / 180.0f);
}

namespace Quartz
{
	namespace Math
	{
		FORCEINLINE float MagnitudeSquared(float x, float y)
		{
			return (x * x) + (y * y);
		}

		FORCEINLINE float MagnitudeSquared(float x, float y, float z)
		{
			return (x * x) + (y * y) + (z * z);
		}

		FORCEINLINE float MagnitudeSquared(float x, float y, float z, float w)
		{
			return (x * x) + (y * y) + (z * z) + (w * w);
		}

		FORCEINLINE float Magnitude(float x, float y)
		{
			return 1.0f / FastInvsereSquare(MagnitudeSquared(x, y));
		}

		FORCEINLINE float Magnitude(float x, float y, float z)
		{
			return 1.0f / FastInvsereSquare(MagnitudeSquared(x, y, z));
		}

		FORCEINLINE float Magnitude(float x, float y, float z, float w)
		{
			return 1.0f / FastInvsereSquare(MagnitudeSquared(x, y, z, w));
		}
	}
}