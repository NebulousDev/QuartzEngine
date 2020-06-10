#pragma once

#include "Common.h"
#include "../PlatformConsole.h"
#include "util\Singleton.h"
#include "../PlatformWindow.h"
#include "System.h"

namespace Quartz
{
	class QUARTZ_API SystemImpl
	{
	public:
		virtual void Create() = 0;

		virtual const char* GetPlatformName() = 0;

		virtual PlatformConsole* CreateConsole() = 0;

		virtual Window* CreateWindow() = 0;
		virtual Window* CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL) = 0;
		virtual Window* CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL) = 0;

		virtual Time64 GetTimeNanoseconds() = 0;
		virtual Time64 GetUptimeNanoseconds() = 0;
	};

	class QUARTZ_API PlatformSystem
	{
	private:
		static SystemImpl* mpSystemImpl;

		PlatformSystem() = default;

	public:
		static void Create(SystemImpl* pPlatformLayer);

		static PlatformConsole* CreateConsole();

		static Window* CreateWindow();
		static Window* CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL);
		static Window* CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL);

		static Time64 GetTimeNanoseconds();
		static Time64 GetUptimeNanoseconds();
	};
}