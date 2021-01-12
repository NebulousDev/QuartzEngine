#pragma once

#include "platform/VPPlatform.h"
//#include "Win32PlatformInput.h"
//#include "Win32PlatformTime.h"
#include "Win32PlatformConsole.h"
#include "Win32Window.h"

#include "util/Map.h"

namespace Quartz
{
	class QUARTZ_API Win32Platform : public VPPlatform
	{
	private:
		HINSTANCE			mInstance;
		//Win32PlatformInput	mWin32PlatformInput;
		//Win32PlatformTime	mWin32PlatformTime;

		// @Note: I cannot use HWND here as I would like
		//	because void* cannot be used in a hash function
		//	for whatever reason...
		Map<Handle64, Win32Window*> mHWNDMap;

	private:
		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		void PreInitialize() override;
		void Initialize() override;

		VPDebugConsole* CreateDebugConsole() override;
		void DestroyDebugConsole(VPDebugConsole* pDebugConsole) override;

		HVPWindow CreateWindow(UInt32 posX, UInt32 posY,
			UInt32 clientWidth, UInt32 clientHeight, const StringW& title) override;

		void DestroyWindow(HVPWindow window) override;

		void PollEvents() override;

		//PlatformInput& GetPlatformInput() override;
		//PlatformTime& GetPlatformTime() override;
	};
}