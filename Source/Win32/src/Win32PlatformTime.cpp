#include "Win32PlatformTime.h"

#include "Win32.h"

namespace Quartz
{
	Win32PlatformTime::Win32PlatformTime()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		mFrequency = static_cast<Double64>(frequency.QuadPart);
		mNanosecondDivisor	= 1.0 / mFrequency;
		mMicrosecondDivisor = 1000.0 / mFrequency;
		mMillisecondDivisor = 1000000.0 / mFrequency;
		mSecondDivisor		= 1000000000.0 / mFrequency;
	}

	Time64 Quartz::Win32PlatformTime::GetTimeNanoseconds()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return static_cast<Time64>(time.QuadPart * mNanosecondDivisor);
	}

	Time64 Win32PlatformTime::GetTimeMicroseconds()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return static_cast<Time64>(time.QuadPart * mMicrosecondDivisor);
	}

	Time64 Win32PlatformTime::GetTimeMilliseconds()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return static_cast<Time64>(time.QuadPart * mMillisecondDivisor);
	}

	Time64 Win32PlatformTime::GetTimeSeconds()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return static_cast<Time64>(time.QuadPart * mSecondDivisor);
	}
}