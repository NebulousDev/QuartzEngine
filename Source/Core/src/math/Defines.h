#pragma once

//////////////////////////////////////////////////////////////
// Preprocessor Definitions
//////////////////////////////////////////////////////////////

/* Compiler Definitions */

#ifdef _MSC_VER
#define COMPILER_MSC
#endif

#ifdef __GNUC__
#define COMPILER_GCC
#endif

/* Utility */

#ifdef COMPILER_MSC

#define ALIGN(x) __declspec(align(x))
#define FORCEINLINE __forceinline

#elif COMPILER_GCC

#define ALIGN(x) __attribute__((aligned(x)))
#define FORCEINLINE __attribute__((always_inline))

#else

#define ALIGN(x)
#define FORCEINLINE

#endif
