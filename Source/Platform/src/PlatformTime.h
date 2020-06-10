#pragma once

#include "Common.h"

namespace Quartz
{
	class QUARTZ_API PlatformTime
	{
	public:

		/** Get system time in nanoseconds */
		virtual Time64 GetTimeNanoseconds() = 0;

		/** Get system time in microseconds */
		virtual Time64 GetTimeMicroseconds() = 0;

		/** Get system time in miliseconds */
		virtual Time64 GetTimeMilliseconds() = 0;

		/** Get system time in seconds */
		virtual Time64 GetTimeSeconds() = 0;
	};
}