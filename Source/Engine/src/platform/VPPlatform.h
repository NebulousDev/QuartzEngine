#pragma once

#include "Common.h"
//#include "Input.h"
//#include "Time.h"
#include "Console.h"

#include "util/String.h"

namespace Quartz
{
	/* Virtual Platform Objects */

	struct VPResource {};

	struct VPWindow;

	/* Virtual Platform Handles */

	typedef VPResource* HVPWindow;

	/* Null Handles */

#define HGFX_NULL_HANDLE (VPResource*)nullptr

	class QUARTZ_API VPPlatform
	{
	public:
		virtual void PreInitialize() = 0;
		virtual void Initialize() = 0;

		virtual VPDebugConsole* CreateDebugConsole() = 0;
		virtual void DestroyDebugConsole(VPDebugConsole* pDebugConsole) = 0;

		virtual HVPWindow CreateWindow(UInt32 posX, UInt32 posY, 
			UInt32 clientWidth, UInt32 clientHeight, const StringW& title) = 0;

		virtual void DestroyWindow(HVPWindow window) = 0;

		virtual void PollEvents() = 0;

		//virtual PlatformInput& GetPlatformInput() = 0;
		//virtual PlatformTime& GetPlatformTime() = 0;
	};
}