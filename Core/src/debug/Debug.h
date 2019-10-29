#pragma once

#include <assert.h>

#ifdef QUARTZ_DEBUG
#define DEBUG_ASSERT(x) assert(x)
#define DEBUG_ASSERT_NOT_NULL(x) assert(x != nullptr)
#else
#define DEBUG_ASSERT(x)
#define DEBUG_ASSERT_NOT_NULL(x)
#endif