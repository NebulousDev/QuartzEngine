#include "Timer.h"

namespace Quartz
{
	Timer::Timer()
		: mStartTime(0), mLastTime(0)
	{ }

	Time64 Timer::Start()
	{
		PlatformTime& time = PlatformTime::GetInstance();
		mStartTime = time.GetTimeNanoseconds();
		mLastTime = mStartTime;
		return mStartTime;
	}

	Time64 Timer::Reset()
	{
		PlatformTime& time = PlatformTime::GetInstance();
		mLastTime = time.GetTimeNanoseconds();
		return mLastTime;
	}

	Time64 Timer::GetStartTime()
	{
		return mStartTime;
	}

	Time64 Timer::GetLapsedTime()
	{
		PlatformTime& time = PlatformTime::GetInstance();
		return time.GetTimeNanoseconds() - mStartTime;
	}

	Time64 Timer::GetLastTime()
	{
		return mLastTime;
	}
}

