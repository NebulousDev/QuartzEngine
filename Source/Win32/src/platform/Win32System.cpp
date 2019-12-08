#include "Win32System.h"

#include "..\io\Win32Console.h"
#include "Win32Window.h"

namespace Quartz
{
	void Win32System::Init()
	{
		LARGE_INTEGER frequency;
		LARGE_INTEGER startTime;

		QueryPerformanceFrequency(&frequency);

		mTimeFrequency = static_cast<Double64>(frequency.QuadPart);
		mNanosecondDivisor = 1.0 / mTimeFrequency;

		QueryPerformanceCounter(&startTime);

		mStartTime = static_cast<Time64>(startTime.QuadPart) * mNanosecondDivisor;
	}

	const char* Win32System::GetPlatformName()
	{
		return "Windows";
	}

	SystemConsole* Win32System::CreateConsole()
	{
		return new Win32SystemConsole();
	}

	Window* Win32System::CreateWindow()
	{
		return new Win32Window();
	}

	Window* Win32System::CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags)
	{
		return new Win32Window(width, height, title);
	}

	Window* Win32System::CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags)
	{
		return new Win32Window(xPos, yPos, width, height, title);
	}

	Time64 Win32System::GetTimeNanoseconds()
	{
		LARGE_INTEGER time;
		QueryPerformanceCounter(&time);
		return static_cast<Time64>(time.QuadPart) * mNanosecondDivisor;
	}

	Time64 Win32System::GetUptimeNanoseconds()
	{
		return GetTimeNanoseconds() - mStartTime;
	}
}

