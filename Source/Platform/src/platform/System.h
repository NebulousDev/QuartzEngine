#pragma once

#include "Common.h"
#include "..\io\Console.h"
#include "util\Singleton.h"
#include "Window.h"
#include "System.h"

namespace Quartz
{
	class QUARTZ_API SystemImpl
	{
	public:
		virtual void Init() = 0;

		virtual const char* GetPlatformName() = 0;

		virtual SystemConsole* CreateConsole() = 0;

		virtual Window* CreateWindow() = 0;
		virtual Window* CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL) = 0;
		virtual Window* CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL) = 0;

		virtual Time64 GetTimeNanoseconds() = 0;
		virtual Time64 GetUptimeNanoseconds() = 0;
	};

	class QUARTZ_API System : public Singleton<System>
	{
	public:
		friend class Singleton<System>;

	private:
		static SystemImpl* mpSystemImpl;

		System() = default;

	public:
		static void Init(SystemImpl* pPlatformLayer);

		static SystemConsole* CreateConsole();

		static Window* CreateWindow();
		static Window* CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL);
		static Window* CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL);

		static Time64 GetTimeNanoseconds();
		static Time64 GetUptimeNanoseconds();
	};
}