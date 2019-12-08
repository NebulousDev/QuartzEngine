#include "System.h"

namespace Quartz
{
	SystemImpl* System::mpSystemImpl = NULL;

	void System::Init(SystemImpl* pPlatformLayer)
	{
		mpSystemImpl = pPlatformLayer;
		mpSystemImpl->Init();
	}

	SystemConsole* System::CreateConsole()
	{
		return mpSystemImpl->CreateConsole();
	}

	Window* System::CreateWindow()
	{
		return mpSystemImpl->CreateWindow();
	}

	Window* System::CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags)
	{
		return mpSystemImpl->CreateWindow(width, height, title, flags);
	}

	Window* System::CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags)
	{
		return mpSystemImpl->CreateWindow(xPos, yPos, width, height, title, flags);
	}

	Time64 System::GetTimeNanoseconds()
	{
		return mpSystemImpl->GetTimeNanoseconds();
	}

	Time64 System::GetUptimeNanoseconds()
	{
		return mpSystemImpl->GetUptimeNanoseconds();
	}
}

