#pragma once

#include "../Common.h"

#include "Matrix.h"
#include "Quaternion.h"
#include "Vector.h"

namespace Quartz
{
	FORCE_INLINE Quaternion Slerp(Quaternion quat1, Quaternion quat2, Float32 value)
	{
		Float32 halfAngle = quat1.Dot(quat2);
		
		Float32 sinHalf = sinf(halfAngle);
		Float32 c1 = sinf((1 - value) * halfAngle) / sinHalf;
		Float32 c2 = sinf(value * halfAngle) / sinHalf;

		return quat1 * c1 + quat2 * c2;
	}
}