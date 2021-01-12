#pragma once

#include "platform/Platform.h"
#include "Win32PlatformInput.h"
#include "Win32PlatformTime.h"
#include "Win32PlatformConsole.h"
//#include "Win32PlatformWindow.h"

namespace Quartz
{
	class QUARTZ_API Win32Platform : public Platform
	{
	private:
		Win32PlatformInput	mWin32PlatformInput;
		Win32PlatformTime	mWin32PlatformTime;

	public:
		void Initialize() override;

		PlatformConsole* CreateConsole() override;

		//Window* CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL) override;
		//Window* CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL) override;

		PlatformInput& GetPlatformInput() override;
		PlatformTime& GetPlatformTime() override;
	};
}