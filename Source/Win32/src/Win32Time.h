#pragma once

#include "platform/Time.h"

namespace Quartz
{
	class QUARTZ_API Win32Time : public Time
	{
	private:
		Double64 mFrequency;
		Double64 mNanosecondDivisor;
		Double64 mMicrosecondDivisor;
		Double64 mMillisecondDivisor;
		Double64 mSecondDivisor;

	public:
		Win32Time();

		/** Get system time in nanoseconds */
		Time64 GetTimeNanoseconds() override;

		/** Get system time in microseconds */
		Time64 GetTimeMicroseconds() override;

		/** Get system time in miliseconds */
		Time64 GetTimeMilliseconds() override;

		/** Get system time in seconds */
		Time64 GetTimeSeconds() override;
	};
}