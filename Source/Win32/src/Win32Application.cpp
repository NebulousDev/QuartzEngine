#include "Win32Application.h"

#include "Win32Window.h"
#include "Win32Util.h"
#include "log/Log.h"

namespace Quartz
{
	Win32Application::Win32Application(const ApplicationInfo& info, const Win32ApplicationInfo& win32info)
		: Application(info), mWin32Info(win32info)
	{
		// Nothing
	}

	static void PrintLastError()
	{
		DWORD error = GetLastError();

		LPWSTR pErrorMessage = nullptr;

		FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&pErrorMessage,
			0, NULL);

		if (pErrorMessage == nullptr)
		{
			// No error
			return;
		}

		Log::Error(pErrorMessage);

		LocalFree(pErrorMessage);
	}

	LRESULT CALLBACK Win32Application::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Win32Application* pApp = (Win32Application*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (pApp != nullptr)
		{
			Win32Window* pWindow = pApp->mHWNDMap[reinterpret_cast<Handle64>(hwnd)];

			if (pWindow != nullptr)
			{
				switch (uMsg)
				{
				case WM_KILLFOCUS:
				{
					/*
					if (pApp->mWindowFocusCallback)
					{
						pWindow->focused = false;
						pApp->mWindowFocusCallback(pWindow, false);
					}

					if (pApp->mCapturingWindow)
					{
						pApp->ReleaseCursor();
					}
					*/

					break;
				}

				case WM_SETFOCUS:
				{
					/*
					if (pApp->mWindowFocusCallback)
					{
						pWindow->focused = true;
						pApp->mWindowFocusCallback(pWindow, true);
					}
					*/

					break;
				}

				case WM_MOUSEMOVE:
				{
					/*
					if (!pWindow->mouseInside)
					{
						if (pApp->mWindowMouseEnteredCallback)
						{
							pWindow->mouseInside = true;
							pApp->mWindowMouseEnteredCallback(pWindow, true);
						}
					}
					*/

					break;
				}

				case WM_MOUSELEAVE:
				{
					/*
					if (pApp->mWindowMouseEnteredCallback)
					{
						pWindow->mouseInside = false;
						pApp->mWindowMouseEnteredCallback(pWindow, false);
					}
					*/

					break;
				}

				case WM_SIZE:
				{
					/*
					UInt32 x = LOWORD(lParam);
					UInt32 y = HIWORD(lParam);

					RECT rect, clientRect;
					GetWindowRect(hwnd, &rect);
					GetClientRect(hwnd, &clientRect);
					ClientToScreen(hwnd, reinterpret_cast<POINT*>(&clientRect.left));
					ClientToScreen(hwnd, reinterpret_cast<POINT*>(&clientRect.right));

					Bounds2i bounds(rect.left, rect.top, rect.right, rect.bottom);
					Bounds2i clientBounds(clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);

					pWindow->bounds = bounds;
					pWindow->clientBounds = clientBounds;

					if (pApp->mWindowResizedCallback)
					{
						pApp->mWindowResizedCallback(pWindow, clientBounds.Width(), clientBounds.Height());
					}

					if (pApp->mCapturingWindow)
					{
						// @Note: Recapture mouse to reset clipping 
						pApp->CaptureCursor(pApp->mCapturingWindow);
					}
					*/

					break;
				}
				case WM_MOVE:
				{
					/*
					UInt32 x = LOWORD(lParam);
					UInt32 y = HIWORD(lParam);

					RECT rect, clientRect;
					GetWindowRect(hwnd, &rect);
					GetClientRect(hwnd, &clientRect);
					ClientToScreen(hwnd, reinterpret_cast<POINT*>(&clientRect.left));
					ClientToScreen(hwnd, reinterpret_cast<POINT*>(&clientRect.right));

					Bounds2i bounds(rect.left, rect.top, rect.right, rect.bottom);
					Bounds2i clientBounds(clientRect.left, clientRect.top, clientRect.right, clientRect.bottom);

					pWindow->bounds = bounds;
					pWindow->clientBounds = clientBounds;

					if (pApp->mWindowMovedCallback)
					{
						pApp->mWindowMovedCallback(pWindow, clientBounds.start.x, clientBounds.start.y);
					}

					if (pApp->mCapturingWindow)
					{
						// @Note: Recapture mouse to reset clipping 
						pApp->CaptureCursor(pApp->mCapturingWindow);
					}
					*/

					break;
				}
				case WM_DESTROY:
				{
					//pApp->ReleaseCursor();

					break;
				}
				default:
					break;
				}
			}
		}

		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	Window* Win32Application::CreateWindow(const WindowInfo& info)
	{
		DWORD dwStyle	= 0; 
		UInt32 width	= info.size.x;
		UInt32 height	= info.size.y;

		switch(info.mode)
		{
			case WINDOW_MODE_WINDOWED:
			{
				dwStyle = WS_OVERLAPPEDWINDOW;
				break;
			}

			case WINDOW_MODE_BORDERLESS:
			{
				dwStyle = WS_POPUP;
				break;
			}

			case WINDOW_MODE_FULLSCREEN:
			{
				dwStyle = WS_POPUP;
				width	= GetSystemMetrics(SM_CXSCREEN);
				height	= GetSystemMetrics(SM_CYSCREEN);
				break;
			}
		}

		RECT windowSize =
		{ 
			info.position.x, info.position.y,
			info.position.x + width,
			info.position.y + height
		};

		AdjustWindowRect(&windowSize, dwStyle, FALSE);

		UInt32 adjustedWidth	= windowSize.right - windowSize.left;
		UInt32 adjustedHeight	= windowSize.bottom - windowSize.top;
		Int32 adjustedPosX		= windowSize.left;
		Int32 adjustedPosY		= windowSize.top;

		DWORD dwStyleAndVisible = dwStyle | (info.shown ? WS_VISIBLE : NULL);

		HWND hwnd = CreateWindowExW(0, mWin32Info.wndClass.lpszClassName, (LPCWSTR)info.title.Str(), 
			dwStyleAndVisible, adjustedPosX, adjustedPosY, adjustedWidth, adjustedHeight,
			NULL, NULL, mWin32Info.hInstance, NULL);

		if (hwnd == NULL)
		{
			Log::Critical(L"Unable to create window '%s'! CreateWindowEx() returned null.", info.title.Str());
			PrintLastError();
			return nullptr;
		}

		Win32WindowInfo win32info;
		win32info.hwnd = hwnd;
		win32info.dwStyle = dwStyle;

		Win32Window* pWindow = new Win32Window(info, win32info);

		mHWNDMap[reinterpret_cast<Handle64>(hwnd)] = pWindow;

		SetWindowLongPtrW(hwnd, GWLP_USERDATA, (LONG_PTR)this);

		pWindow->Focus(info.focused);

		return pWindow;
	}

	void Win32Application::PollMessages()
	{
		MSG msg = {};

		// Process and remove all messages before WM_INPUT
		while (PeekMessageW(&msg, NULL, 0, WM_INPUT - 1, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		// Process and remove all messages after WM_INPUT
		while (PeekMessageW(&msg, NULL, WM_INPUT + 1, (UINT)-1, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
}

