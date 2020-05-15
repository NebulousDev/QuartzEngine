#pragma once

//////////////////////////////////////////////////////////////
// Math Definitions
//////////////////////////////////////////////////////////////

#define USE_SIMD 0
#define FORCE_DISABLE_SIMD 0

#include "MathUtil.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

#if USE_SIMD & !FORCE_DISABLE_SIMD

#else

#include "Vector2Impl.h"
#include "Vector3Impl.h"
#include "Vector4Impl.h"
#include "Matrix4Impl.h"
#include "QuaternionImpl.h"

#endif
