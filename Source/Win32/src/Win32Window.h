#pragma once

#include "Win32.h"
#include "platform/Window.h"

namespace Quartz
{
	struct Win32WindowInfo
	{
		HWND	hwnd;
		DWORD	dwStyle;
	};

	class QUARTZ_API Win32Window : public Window
	{
	private:
		Win32WindowInfo mWin32Info;

	public:
		Win32Window(const WindowInfo& info, const Win32WindowInfo& win32Info);

		void Move(UInt32 x, UInt32 y) override;
		void Resize(UInt32 width, UInt32 height) override;
		void Focus(Bool8 focus) override;
		void Show(Bool8 show) override;

		FORCE_INLINE HWND	GetHWND() { return mWin32Info.hwnd; }
		FORCE_INLINE DWORD	GetStyle() { return mWin32Info.dwStyle; }
	};
}