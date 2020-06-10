#pragma once

#include "platform\System.h"

namespace Quartz
{
	class QUARTZ_API Win32System : public SystemImpl
	{
	private:
		Time64 mStartTime;
		Double64 mTimeFrequency;
		Double64 mNanosecondDivisor;

	public:
		void Create() override;

		const char* GetPlatformName() override;

		PlatformConsole* CreateConsole() override;

		Window* CreateWindow() override;
		Window* CreateWindow(UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL) override;
		Window* CreateWindow(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title, Flags16 flags = NULL) override;

		Time64 GetTimeNanoseconds() override;
		Time64 GetUptimeNanoseconds() override;
	};
}