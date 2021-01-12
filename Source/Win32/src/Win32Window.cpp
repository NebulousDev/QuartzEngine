#include "Win32Window.h"

namespace Quartz
{
	Win32Window::Win32Window(HWND handle, const StringW& title, Bounds2i bounds, Bounds2i clientBounds, 
		Bool8 createdFullscreen, Bool8 createdMinimized, Bool8 createdMaximized, Bool8 createdVisible)
		: Window(title, bounds, clientBounds, createdFullscreen, createdMinimized, createdMaximized, createdVisible),
		mHandle(handle) { }

	void Win32Window::SetPosition(Int32 x, Int32 y)
	{
		SetWindowPos(mHandle, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	void Win32Window::SetClientPosition(Int32 x, Int32 y)
	{
		RECT windowSize = { x, y, 0, 0 };
		AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);
		SetPosition(windowSize.left, windowSize.top);
	}

	void Win32Window::SetTitle(const StringW& title)
	{
		SetWindowTextW(mHandle, title.Str());
	}

	void Win32Window::SetSize(UInt32 width, UInt32 height)
	{
		SetWindowPos(mHandle, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	void Win32Window::SetClientSize(UInt32 width, UInt32 height)
	{
		RECT windowSize = { 0, 0, width, height };
		AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);
		SetPosition(windowSize.right - windowSize.left, windowSize.bottom - windowSize.top);
	}

	void Win32Window::BringToFront()
	{
		SetWindowPos(mHandle, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	void Win32Window::Minimize()
	{
		ShowWindow(mHandle, SW_MINIMIZE);
	}

	void Win32Window::Maximize()
	{
		ShowWindow(mHandle, SW_MAXIMIZE);
	}

	void Win32Window::Focus()
	{
		SetFocus(mHandle);
	}

	void Win32Window::Show()
	{
		ShowWindow(mHandle, SW_SHOWNOACTIVATE);
	}

	void Win32Window::Hide()
	{
		ShowWindow(mHandle, SW_HIDE);
	}

	void* Win32Window::GetNativeHandle()
	{
		return GetHWND();
	}

	HWND Win32Window::GetHWND() const
	{
		return mHandle;
	}
}

