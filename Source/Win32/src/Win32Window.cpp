#include "Win32Window.h"
#include "Win32Window.h"
#include "Win32Window.h"
#include "Win32Window.h"
#include "Win32Window.h"

namespace Quartz
{
	Win32Window::Win32Window(const WindowInfo& info, const Win32WindowInfo& win32Info)
		: Window(info), mWin32Info(win32Info)
	{
		// Nothing
	}

	void Win32Window::Move(UInt32 x, UInt32 y)
	{
		RECT windowSize = { x, y, 0, 0 };
		AdjustWindowRect(&windowSize, mWin32Info.dwStyle, FALSE);
		SetWindowPos(mWin32Info.hwnd, HWND_TOP, windowSize.left, windowSize.top, 
			0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	void Win32Window::Resize(UInt32 width, UInt32 height)
	{
		RECT windowSize = { 0, 0, width, height };
		AdjustWindowRect(&windowSize, mWin32Info.dwStyle, FALSE);
		SetWindowPos(mWin32Info.hwnd, HWND_TOP, 0, 0, 
			windowSize.right - windowSize.left, windowSize.bottom - windowSize.top, 
			SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	void Win32Window::Focus(Bool8 focus)
	{
		if (focus)
		{
			SetFocus(mWin32Info.hwnd);
		}
		else
		{
			SetFocus(NULL);
		}
	}

	void Win32Window::Show(Bool8 show)
	{
		if (show)
		{
			ShowWindow(mWin32Info.hwnd, SW_SHOWNOACTIVATE);
		}
		else
		{
			ShowWindow(mWin32Info.hwnd, SW_HIDE);
		}
	}

	/*
	void Win32Window::SetPosition(Int32 x, Int32 y)
	{
		SetWindowPos(mHWND, HWND_TOP, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	void Win32Window::SetClientPosition(Int32 x, Int32 y)
	{
		RECT windowSize = { x, y, 0, 0 };
		AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);
		SetPosition(windowSize.left, windowSize.top);
	}

	void Win32Window::SetTitle(const StringW& title)
	{
		SetWindowTextW(mHWND, title.Str());
	}

	void Win32Window::SetSize(UInt32 width, UInt32 height)
	{
		SetWindowPos(mHWND, HWND_TOP, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
	}

	void Win32Window::SetClientSize(UInt32 width, UInt32 height)
	{
		RECT windowSize = { 0, 0, width, height };
		AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);
		SetPosition(windowSize.right - windowSize.left, windowSize.bottom - windowSize.top);
	}

	void Win32Window::BringToFront()
	{
		SetWindowPos(mHWND, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	void Win32Window::Minimize()
	{
		ShowWindow(mHWND, SW_MINIMIZE);
	}

	void Win32Window::Maximize()
	{
		ShowWindow(mHWND, SW_MAXIMIZE);
	}

	void Win32Window::Focus()
	{
		SetFocus(mHWND);
	}

	void Win32Window::Show()
	{
		ShowWindow(mHWND, SW_SHOWNOACTIVATE);
	}

	void Win32Window::Hide()
	{
		ShowWindow(mHWND, SW_HIDE);
	}

	void* Win32Window::GetNativeHandle()
	{
		return GetHWND();
	}

	HWND Win32Window::GetHWND() const
	{
		return mHWND;
	}

	const StringW& Win32Window::Title()
	{
		return mTitle;
	}

	Bounds2i Win32Window::Bounds()
	{
		return mBounds;
	}

	Bounds2i Win32Window::ClientBounds()
	{
		return mClientBounds;
	}

	Point2i Win32Window::Position()
	{
		return mBounds.start;
	}

	Point2i Win32Window::Size()
	{
		return Point2i(Width(), Height());
	}

	Point2i Win32Window::ClientSize()
	{
		return Point2i(ClientWidth(), ClientHeight());
	}

	Point2i Win32Window::Center()
	{
		return Point2i(mClientBounds.start.x + mClientBounds.Width() / 2,
			mClientBounds.start.y + mClientBounds.Height() / 2);
	}

	UInt32 Win32Window::Width()
	{
		return mBounds.Width();
	}

	UInt32 Win32Window::Height()
	{
		return mBounds.Height();
	}

	UInt32 Win32Window::ClientWidth()
	{
		return mClientBounds.Width();
	}

	UInt32 Win32Window::ClientHeight()
	{
		return mClientBounds.Height();
	}

	Bool8 Win32Window::IsMaximized()
	{
		return mMaximized;
	}

	Bool8 Win32Window::IsMinimized()
	{
		return mMinimized;
	}

	Bool8 Win32Window::IsFullscreen()
	{
		return mFullscreen;
	}

	Bool8 Win32Window::IsVisible()
	{
		return mVisible;
	}
	*/
}

