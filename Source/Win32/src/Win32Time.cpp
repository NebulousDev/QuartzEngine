#include "Win32Time.h"

#include "Win32.h"

namespace Quartz
{
	Win32Time::Win32Time()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		mFrequency = static_cast<Double64>(frequency.QuadPart);
		mNanosecondDivisor	= 1.0 / mFrequency;
		mMicrosecondDivisor = 1000.0 / mFrequency;
		mMillisecondDivisor = 1000000.0 / mFrequency;
		mSecondDivisor		= 1000000000.0 / mFrequency;
	}

	Time64 Win32Time::GetTimeNanoseconds()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return static_cast<Time64>(time.QuadPart * mNanosecondDivisor);
	}

	Time64 Win32Time::GetTimeMicroseconds()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return static_cast<Time64>(time.QuadPart * mMicrosecondDivisor);
	}

	Time64 Win32Time::GetTimeMilliseconds()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return static_cast<Time64>(time.QuadPart * mMillisecondDivisor);
	}

	Time64 Win32Time::GetTimeSeconds()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return static_cast<Time64>(time.QuadPart * mSecondDivisor);
	}
}