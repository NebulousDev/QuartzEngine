#include "Win32Platform.h"

namespace Quartz
{
	void Win32Platform::Initialize()
	{
		//mWin32PlatformInput.InitializeInput();
	}

	PlatformConsole* Win32Platform::CreateConsole()
	{
		return new Win32Console();
	}

	Window* Win32Platform::CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags)
	{
		return new Win32Window(width, height, title);
	}

	Window* Win32Platform::CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags)
	{
		return new Win32Window(xPos, yPos, width, height, title);
	}

	PlatformInput& Win32Platform::GetPlatformInput()
	{
		return mWin32PlatformInput;
	}

	PlatformTime& Win32Platform::GetPlatformTime()
	{
		return mWin32PlatformTime;
	}
}

