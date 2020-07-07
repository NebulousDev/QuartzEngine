#include "Win32PlatformWindow.h"

#include "Win32.h"
//#include "io/Log.h"
#include "util\Array.h"

#include <hidsdi.h>
#pragma comment(lib,"Hid.lib")

#define MAX_INPUT_BUFFER_SIZE 16

namespace Quartz
{
	Bool8 Win32Window::ShowImpl()
	{
		ShowWindow((HWND)mHandle, SW_SHOW);
		return true;
	}

	Bool8 Win32Window::HideImpl()
	{
		ShowWindow((HWND)mHandle, SW_HIDE);
		return true;
	}

	Bool8 Win32Window::SetPositionImpl(UInt32 xPos, UInt32 yPos)
	{
		return Bool8();
	}

	Bool8 Win32Window::SetSizeImpl(UInt32 width, UInt32 height)
	{
		return Bool8();
	}

	Bool8 Win32Window::SetTitleImpl(const char* title)
	{
		return Bool8();
	}

	WindowHandle Win32Window::GetNativeHandleImpl()
	{
		return (WindowHandle) mHandle;
	}

	void Win32Window::Update()
	{
		MSG msg = {};

		//Process and remove all messages before WM_INPUT
		while (PeekMessage(&msg, NULL, 0, WM_INPUT - 1, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//Process and remove all messages after WM_INPUT
		while (PeekMessage(&msg, NULL, WM_INPUT + 1, (UINT)-1, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	Win32Window::Win32Window()
		: Win32Window(0, 0, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, DEFAULT_WINDOW_TITLE)
	{ }

	Win32Window::Win32Window(UInt32 width, UInt32 height, const char* title)
		: Win32Window(0, 0, width, height, title)
	{ }

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Win32Window::Win32Window(UInt32 xPos, UInt32 yPos, UInt32 width, UInt32 height, const char* title)
	{
		HINSTANCE instance = GetModuleHandle(NULL);
		const char className[] = "Quartz Game Window";

		WNDCLASS winClass = {};
		winClass.lpfnWndProc = WindowProc;
		winClass.hInstance = instance;
		winClass.lpszClassName = className;

		ATOM result = RegisterClass(&winClass);
		
		if(result == 0)
		{
			//Log.Critical("Failed to register window class!");
			return;
		}

		RECT windowSize = { 0, 0, width, height };
		AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);

		UInt32 adjustedWidth = windowSize.right - windowSize.left;
		UInt32 adjustedHeight = windowSize.bottom - windowSize.top;

		HWND windowHandle = CreateWindowEx(0, className, title, WS_OVERLAPPEDWINDOW,
			xPos, yPos, adjustedWidth, adjustedHeight, NULL, NULL, instance, NULL);

		if (windowHandle == NULL)
		{
			//Log.Critical("Failed to create window!");
			return;
		}

		mPosX = xPos;
		mPosY = yPos;
		mWidth = width;
		mHeight = height;
		mTitle = title;

		mHandle = (Win32WindowHandle)windowHandle;
	}

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}