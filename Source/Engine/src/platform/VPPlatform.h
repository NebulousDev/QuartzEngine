#pragma once

#include "Common.h"
#include "util/Array.h"
#include "util/String.h"

#include "VPDebugConsole.h"

namespace Quartz
{
	/* Virtual Platform Objects */

	struct VPResource {};

	struct VPWindow;

	/* Virtual Platform Handles */

	typedef VPResource* HVPWindow;

	/* Null Handles */

#define VP_NULL_HANDLE nullptr

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
	};
}