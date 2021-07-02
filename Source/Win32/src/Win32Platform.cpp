#include "Win32Platform.h"
#include "Win32Window.h"

#include "Win32Application.h"

#include "Win32Util.h"
#include "log/Log.h"

#include <fcntl.h>
#include <stdio.h>
#include <io.h>

namespace Quartz
{
	Win32Platform::Win32Platform()
		: Platform({ L"Windows Platform" })
	{
		// Nothing
	}

	DebugConsole* Win32Platform::CreateDebugConsole()
	{
		AllocConsole();

		HANDLE	std		= GetStdHandle(STD_OUTPUT_HANDLE);
		Int32	crt		= _open_osfhandle((intptr_t)std, _O_TEXT);
		FILE*	pStream = _fdopen(crt, "w");

		setvbuf(pStream, NULL, _IONBF, 1);
		_setmode(_fileno(pStream), _O_U16TEXT);

		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(std, &consoleInfo);

		WORD defaultColor = consoleInfo.wAttributes;

		HWND hwnd = GetConsoleWindow();

		Win32DebugConsoleInfo win32ConsoleInfo;
		win32ConsoleInfo.hwnd			= hwnd;
		win32ConsoleInfo.handle			= std;
		win32ConsoleInfo.crt			= crt;
		win32ConsoleInfo.pOutputStream	= pStream;
		win32ConsoleInfo.defaultColor	= defaultColor;

		return new Win32DebugConsole(win32ConsoleInfo);
	}

	void Win32Platform::DestroyDebugConsole(DebugConsole* pDebugConsole)
	{
		FreeConsole();
		delete pDebugConsole;
	}

	Application* Win32Platform::CreateApplication(const ApplicationInfo& info)
	{
		WNDCLASSW wndClass = {};
		wndClass.lpfnWndProc	= Win32Application::WindowProc;
		wndClass.hInstance		= GetModuleHandle(NULL);
		wndClass.lpszClassName	= info.name.Str();

		ATOM result = RegisterClassW(&wndClass);

		if (result == 0)
		{
			Log::Critical(L"Unable to create application '%s'! RegisterClass() failed.", info.name.Str());
			PrintLastError();
			return nullptr;
		}

		Win32ApplicationInfo win32info;
		win32info.hInstance = GetModuleHandle(NULL);
		win32info.wndClass	= wndClass;

		return new Win32Application(info, win32info);
	}

	Bool8 Win32Platform::DestroyApplication(Application* application)
	{
		return Bool8();
	}

	Time* Win32Platform::GetTime()
	{
		return &mTime;
	}

	/*
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Win32Platform* pPlatform = (Win32Platform*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (pPlatform != nullptr)
		{
			Win32Window* pWindow = pPlatform->mHWNDMap[reinterpret_cast<Handle64>(hwnd)];

			if (pWindow != nullptr)
			{
				switch (uMsg)
				{
					case WM_KILLFOCUS:
					{
						if (pPlatform->mWindowFocusCallback)
						{
							pWindow->focused = false;
							pPlatform->mWindowFocusCallback(pWindow, false);
						}

						if (pPlatform->mCapturingWindow)
						{
							pPlatform->ReleaseCursor();
						}

						break;
					}

					case WM_SETFOCUS:
					{
						if (pPlatform->mWindowFocusCallback)
						{
							pWindow->focused = true;
							pPlatform->mWindowFocusCallback(pWindow, true);
						}

						break;
					}

					case WM_MOUSEMOVE:
					{
						if (!pWindow->mouseInside)
						{
							if (pPlatform->mWindowMouseEnteredCallback)
							{
								pWindow->mouseInside = true;
								pPlatform->mWindowMouseEnteredCallback(pWindow, true);
							}
						}

						break;
					}
				
					case WM_MOUSELEAVE:
					{
						if (pPlatform->mWindowMouseEnteredCallback)
						{
							pWindow->mouseInside = false;
							pPlatform->mWindowMouseEnteredCallback(pWindow, false);
						}

						break;
					}

					case WM_SIZE:
					{
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

						if (pPlatform->mWindowResizedCallback)
						{
							pPlatform->mWindowResizedCallback(pWindow, clientBounds.Width(), clientBounds.Height());
						}

						if (pPlatform->mCapturingWindow)
						{
							// @Note: Recapture mouse to reset clipping 
							pPlatform->CaptureCursor(pPlatform->mCapturingWindow);
						}

						break;
					}
					case WM_MOVE:
					{
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

						if (pPlatform->mWindowMovedCallback)
						{
							pPlatform->mWindowMovedCallback(pWindow, clientBounds.start.x, clientBounds.start.y);
						}

						if (pPlatform->mCapturingWindow)
						{
							// @Note: Recapture mouse to reset clipping 
							pPlatform->CaptureCursor(pPlatform->mCapturingWindow);
						}

						break;
					}
					case WM_DESTROY:
					{
						pPlatform->ReleaseCursor();

						break;
					}
					default:
						break;
				}
			}
		}

		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}
	

	HVPWindow Win32Platform::CreateWindow(UInt32 posX, UInt32 posY, UInt32 clientWidth, UInt32 clientHeight, const StringW& title)
	{
		const wchar_t className[] = L"Quartz Game Window";

		WNDCLASSW winClass = {};
		winClass.lpfnWndProc = WindowProc;
		winClass.hInstance = mInstance;
		winClass.lpszClassName = className;

		ATOM result = RegisterClassW(&winClass);

		if (result == 0)
		{
			Log::Critical(L"Unable to create window '%s'! RegisterClass() failed.", title.Str());
			PrintLastError();
			return VP_NULL_HANDLE;
		}

		RECT windowSize = { posX, posY, posX + clientWidth, posY + clientHeight };
		AdjustWindowRect(&windowSize, WS_OVERLAPPEDWINDOW, FALSE);

		UInt32 adjustedWidth = windowSize.right - windowSize.left;
		UInt32 adjustedHeight = windowSize.bottom - windowSize.top;
		Int32 adjustedPosX = windowSize.left;
		Int32 adjustedPosY = windowSize.top;

		HWND windowHandle = CreateWindowExW(0, className, (LPCWSTR)title.Str(), WS_OVERLAPPEDWINDOW,
			adjustedPosX, adjustedPosY, adjustedWidth, adjustedHeight, NULL, NULL, mInstance, NULL);

		if (windowHandle == NULL)
		{
			Log::Critical(L"Unable to create window '%s'! CreateWindowEx() returned null.", title.Str());
			PrintLastError();
			return VP_NULL_HANDLE;
		}

		Bounds2i realBounds(adjustedPosX, adjustedPosY, adjustedWidth, adjustedHeight);
		Bounds2i clientBounds(posX, posY, posX + clientWidth, posY + clientHeight);

		Win32Window* pWindow = new Win32Window;
		pWindow->hwnd = windowHandle;
		pWindow->title = title;
		pWindow->bounds = realBounds;
		pWindow->clientBounds = clientBounds;
		pWindow->maximized = false;
		pWindow->minimized = false;
		pWindow->fullscreen = false;
		pWindow->visible = false;
		pWindow->mouseInside = false;
		pWindow->focused = false;

		mHWNDMap.Put(reinterpret_cast<Handle64>(windowHandle), pWindow);

		SetWindowLongPtrW(windowHandle, GWLP_USERDATA, (LONG_PTR)this);

		return HVPWindow(pWindow);
	}

	void Win32Platform::DestroyWindow(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		::DestroyWindow(pWin32Window->hwnd);
		delete pWin32Window;
	}

	void Win32Platform::MoveWindow(HVPWindow window, UInt32 x, UInt32 y)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		::SetWindowPos(pWin32Window->hwnd, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void Win32Platform::ResizeWindow(HVPWindow window, UInt32 width, UInt32 height)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		::SetWindowPos(pWin32Window->hwnd, NULL, 0, 0, width, height, SWP_NOREPOSITION | SWP_NOZORDER);
	}

	void Win32Platform::FocusWindow(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		SetFocus(pWin32Window->hwnd);
	}

	void Win32Platform::ShowWindow(HVPWindow window, Bool8 show)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		::ShowWindow(pWin32Window->hwnd, show);
	}

	void Win32Platform::SetWindowMovedCallback(VPWindowMovedCallback callback)
	{
		mWindowMovedCallback = callback;
	}

	void Win32Platform::SetWindowResizedCallback(VPWindowResizedCallback callback)
	{
		mWindowResizedCallback = callback;
	}

	void Win32Platform::SetWindowFocusCallback(VPWindowFocusCallback callback)
	{
		mWindowFocusCallback = callback;
	}

	void Win32Platform::SetWindowMouseEnteredCallback(VPWindowMouseEnteredCallback callback)
	{
		mWindowMouseEnteredCallback = callback;
	}

	Point2i Win32Platform::GetWindowPosition(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		return pWin32Window->bounds.start;
	}

	Point2i Win32Platform::GetWindowClientPosition(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		return pWin32Window->clientBounds.start;
	}

	Point2i Win32Platform::GetWindowSize(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		return Point2i(pWin32Window->bounds.Width(), pWin32Window->bounds.Height());
	}

	Point2i Win32Platform::GetWindowClientSize(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		return Point2i(pWin32Window->clientBounds.Width(), pWin32Window->clientBounds.Height());
	}

	Bounds2i Win32Platform::GetWindowBounds(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		return pWin32Window->bounds;
	}

	Bounds2i Win32Platform::GetWindowClientBounds(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		return pWin32Window->clientBounds;
	}

	Bool8 Win32Platform::IsWindowFocus(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		return pWin32Window->focused;
	}

	Bool8 Win32Platform::IsMouseInsideWindow(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		return pWin32Window->mouseInside;
	}

	Bool8 Win32Platform::IsMouseInsideWindowClient(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		return pWin32Window->mouseInside && 
			PointInBounds(GetCursorPosition(), pWin32Window->clientBounds);
	}

	void Win32Platform::PollEvents()
	{
		MSG msg = {};

		//Process and remove all messages before WM_INPUT
		while (PeekMessageW(&msg, NULL, 0, WM_INPUT - 1, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}

		//Process and remove all messages after WM_INPUT
		while (PeekMessageW(&msg, NULL, WM_INPUT + 1, (UINT)-1, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
	*/
}

