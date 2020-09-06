#pragma once

#include "Win32.h"
#include "Win32Window.h"
#include "application/WindowManager.h"

namespace Quartz
{
	class QUARTZ_API Win32WindowManager : public WindowManager
	{
	private:
		typedef Handle64 HWNDHandle;

		Map<HWNDHandle, Win32Window*> mHWNDToWindow;

		friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		void SetWindowBounds(Win32Window& window, Bounds2i bounds, Bounds2i clientBounds);

	public:
		Window& CreateWindow(StringW title, Int32 posX, Int32 posY, UInt32 clientWidth,
			UInt32 clientHeight, Bool8 fullscreen, Bool8 visible) override;

		void Update() override;

		Win32Window* GetWindowFromHWND(HWND hwnd);
	};
}