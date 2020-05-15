#include "Win32Window.h"

#include "../Win32.h"
#include "io/Log.h"

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
		if(PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			GetMessage(&msg, NULL, 0, 0);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		static RAWINPUT inputBuffer[MAX_INPUT_BUFFER_SIZE]{};
		static UInt32 bufferSize = sizeof(RAWINPUT) * MAX_INPUT_BUFFER_SIZE;

		Int32 inputCount = GetRawInputBuffer(inputBuffer, &bufferSize, sizeof(RAWINPUTHEADER));

		/*
		if (inputCount > 0)
		{
			for (UInt32 i = 0; i < inputCount; i++)
			{
				Log.Info("Input MX: %d", inputBuffer[i].data.mouse.lLastX);
				Log.Info("Input MY: %d", inputBuffer[i].data.mouse.lLastY);
				Log.Info("Input VK: %d", inputBuffer[i].data.keyboard.VKey);
			}
		}
		*/
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
			Log.Critical("Failed to register window class!");
			return;
		}

		HWND windowHandle = CreateWindowEx(0, className, title, WS_OVERLAPPEDWINDOW,
			xPos, yPos, width, height, NULL, NULL, instance, NULL);

		if (windowHandle == NULL)
		{
			Log.Critical("Failed to create window!");
			return;
		}

		RAWINPUTDEVICE Rid[4];

		// MOUSE
		Rid[0].usUsagePage = 0x01;
		Rid[0].usUsage = 0x02;
		Rid[0].dwFlags = 0; //RIDEV_NOLEGACY;
		Rid[0].hwndTarget = 0;

		// KEYBOARD
		Rid[1].usUsagePage = 0x01;
		Rid[1].usUsage = 0x06;
		Rid[1].dwFlags = 0; //RIDEV_NOLEGACY;
		Rid[1].hwndTarget = 0;

		// GAMEPAD
		Rid[2].usUsagePage = 0x01;
		Rid[2].usUsage = 0x05;
		Rid[2].dwFlags = 0;
		Rid[2].hwndTarget = 0;

		// JOYSTICK
		Rid[3].usUsagePage = 0x01;
		Rid[3].usUsage = 0x04;
		Rid[3].dwFlags = 0;
		Rid[3].hwndTarget = 0;

		if (RegisterRawInputDevices(Rid, 4, sizeof(Rid[0])) == FALSE)
		{
			Log.Critical("Failed to register device inputs!");
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