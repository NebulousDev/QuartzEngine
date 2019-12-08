#include "Timer.h"

#include "..\platform\System.h"

namespace Quartz
{
	Timer::Timer()
		: mStartTime(0), mLastTime(0)
	{ }

	Time64 Timer::Start()
	{
		mStartTime = System::GetTimeNanoseconds();
		mLastTime = mStartTime;
		return mStartTime;
	}

	Time64 Timer::Reset()
	{
		mLastTime = System::GetTimeNanoseconds();
		return mLastTime;
	}

	Time64 Timer::GetStartTime()
	{
		return mStartTime;
	}

	Time64 Timer::GetLapsedTime()
	{
		return System::GetTimeNanoseconds() - mStartTime;
	}

	Time64 Timer::GetLastTime()
	{
		return mLastTime;
	}
}

