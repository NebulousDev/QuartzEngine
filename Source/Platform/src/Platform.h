#pragma once

#include "Common.h"
#include "PlatformInput.h"
#include "PlatformTime.h"
#include "PlatformConsole.h"
//#include "PlatformWindow.h"

namespace Quartz
{
	class QUARTZ_API Platform
	{
	public:
		virtual void Initialize() = 0;

		virtual PlatformConsole* CreateConsole() = 0;

		//virtual Window* CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL) = 0;
		//virtual Window* CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL) = 0;

		virtual PlatformInput& GetPlatformInput() = 0;
		virtual PlatformTime& GetPlatformTime() = 0;
	};
}