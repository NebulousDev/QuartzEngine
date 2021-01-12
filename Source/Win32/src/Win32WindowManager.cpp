#include "Win32WindowManager.h"

#include "Win32.h"
#include "Win32Window.h"

namespace Quartz
{
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Win32WindowManager* pWindowManager = (Win32WindowManager*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (pWindowManager != nullptr)
		{
			Win32Window* pWindow = pWindowManager->GetWindowFromHWND(hwnd);

			if (pWindow != nullptr)
			{
				switch (uMsg)
				{
				case WM_KILLFOCUS:
				{
					WindowFocusLostCallback callback = pWindowManager->GetFocusLostCallback();

					if (callback)
					{
						callback(*pWindow);
					}

					break;
				}
				case WM_SETFOCUS:
				{
					WindowFocusGainedCallback callback = pWindowManager->GetFocusGainedCallback();

					if (callback)
					{
						callback(*pWindow);
					}

					break;
				}
				case WM_SIZE:
				{
					WindowResizedCallback callback = pWindowManager->GetResizedCallback();

					UInt32 x = LOWORD(lParam);
					UInt32 y = HIWORD(lParam);

					RECT rect, clientRect;
					GetWindowRect(pWindow->GetHWND(), &rect);
					GetClientRect(pWindow->GetHWND(), &clientRect);
					ClientToScreen(pWindow->GetHWND(), reinterpret_cast<POINT*>(&clientRect.left));
					ClientToScreen(pWindow->GetHWND(), reinterpret_cast<POINT*>(&clientRect.right));

					Bounds2i bounds(rect.left, rect.top, rect.right, rect.bottom);
					Bounds2i clientBounds(clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);

					pWindowManager->SetWindowBounds(*pWindow, bounds, clientBounds);

					if (callback)
					{
						callback(*pWindow, Point2u(bounds.Width(), bounds.Height()));
					}

					break;
				}
				case WM_MOVE:
				{
					WindowMovedCallback callback = pWindowManager->GetMovedCallback();

					RECT rect, clientRect;
					GetWindowRect(pWindow->GetHWND(), &rect);
					GetClientRect(pWindow->GetHWND(), &clientRect);
					ClientToScreen(pWindow->GetHWND(), reinterpret_cast<POINT*>(&clientRect.left));
					ClientToScreen(pWindow->GetHWND(), reinterpret_cast<POINT*>(&clientRect.right));

					Bounds2i bounds(rect.left, rect.top, rect.right, rect.bottom);
					Bounds2i clientBounds(clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);

					pWindowManager->SetWindowBounds(*pWindow, bounds, clientBounds);

					if (callback)
					{
						callback(*pWindow, Point2i(bounds.start.x, bounds.start.y));
					}

					break;
				}
				case WM_DESTROY:
				{
					//pWindowManager->DestroyWindow();
					break;
				}
				default:
					break;
				}
			}	
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	void Win32WindowManager::SetWindowBounds(Win32Window& window, Bounds2i bounds, Bounds2i clientBounds)
	{
		window.mBounds = bounds;
		window.mClientBounds = clientBounds;
	}

	Window& Win32WindowManager::CreateWindow(StringW title, Int32 posX, Int32 posY, UInt32 clientWidth, UInt32 clientHeight, Bool8 fullscreen, Bool8 visible)
    {
		HINSTANCE instance = GetModuleHandle(NULL);
		const char className[] = "Quartz Game Window";

		WNDCLASS winClass = {};
		winClass.lpfnWndProc = WindowProc;
		winClass.hInstance = instance;
		winClass.lpszClassName = className;

		ATOM result = RegisterClass(&winClass);

		if (result == 0)
		{
			//Log.Critical("Failed to register window class!");
			//return Window();
		}

		RECT windowSize = { posX, posY, posX + clientWidth, posY + clientHeight };
		AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);

		UInt32 adjustedWidth = windowSize.right - windowSize.left;
		UInt32 adjustedHeight = windowSize.bottom - windowSize.top;
		Int32 adjustedPosX = windowSize.left;
		Int32 adjustedPosY = windowSize.top;

		HWND windowHandle = CreateWindowEx(0, className, (LPCSTR)title.Str(), WS_OVERLAPPEDWINDOW,
			adjustedPosX, adjustedPosY, adjustedWidth, adjustedHeight, NULL, NULL, instance, NULL);

		if (windowHandle == NULL)
		{
			//Log.Critical("Failed to create window!");
			//return;
		}

		Bounds2i realBounds(adjustedPosX, adjustedPosY, adjustedWidth, adjustedHeight);
		Bounds2i clientBounds(posX, posY, clientWidth, clientHeight);

		Win32Window* pWindow = new Win32Window(windowHandle, title, realBounds, clientBounds, fullscreen, false, false, visible);

		mHWNDToWindow.Put((HWNDHandle)windowHandle, pWindow);
		SetWindowLongPtr(windowHandle, GWLP_USERDATA, (LONG_PTR)this);

		return *pWindow;
    }

	Win32Window* Win32WindowManager::GetWindowFromHWND(HWND hwnd)
	{
		Win32Window** ppWin32Window = mHWNDToWindow.Get((HWNDHandle)hwnd);
		return ppWin32Window == nullptr ? nullptr : *ppWin32Window;
	}

    void Win32WindowManager::Update()
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
}

