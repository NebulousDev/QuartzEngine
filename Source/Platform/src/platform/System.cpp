#include "System.h"

namespace Quartz
{
	SystemImpl* PlatformSystem::mpSystemImpl = NULL;

	void PlatformSystem::Create(SystemImpl* pPlatformLayer)
	{
		mpSystemImpl = pPlatformLayer;
		mpSystemImpl->Create();
	}

	PlatformConsole* PlatformSystem::CreateConsole()
	{
		return mpSystemImpl->CreateConsole();
	}

	Window* PlatformSystem::CreateWindow()
	{
		return mpSystemImpl->CreateWindow();
	}

	Window* PlatformSystem::CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags)
	{
		return mpSystemImpl->CreateWindow(width, height, title, flags);
	}

	Window* PlatformSystem::CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags)
	{
		return mpSystemImpl->CreateWindow(xPos, yPos, width, height, title, flags);
	}

	Time64 PlatformSystem::GetTimeNanoseconds()
	{
		return mpSystemImpl->GetTimeNanoseconds();
	}

	Time64 PlatformSystem::GetUptimeNanoseconds()
	{
		return mpSystemImpl->GetUptimeNanoseconds();
	}
}

