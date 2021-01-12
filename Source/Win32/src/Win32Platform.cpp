#include "Win32Platform.h"
#include "log/Log.h"

namespace Quartz
{
	void Win32Platform::PreInitialize()
	{
		Log::Debug("Pre-initializing Windows platform...");

		// Nothing
	}

	void Win32Platform::Initialize()
	{
		Log::Debug("Initializing Windows platform...");
		mInstance = GetModuleHandle(NULL);
	}

	VPDebugConsole* Win32Platform::CreateDebugConsole()
	{
		return new Win32Console();
	}

	void Win32Platform::DestroyDebugConsole(VPDebugConsole* pDebugConsole)
	{
		delete pDebugConsole;
	}

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Win32Platform* pWindowManager = (Win32Platform*)GetWindowLongPtr(hwnd, GWLP_USERDATA);

		if (pWindowManager != nullptr)
		{
			Win32Window* pWindow = pWindowManager->mHWNDMap[reinterpret_cast<Handle64>(hwnd)];

			if (pWindow != nullptr)
			{
				switch (uMsg)
				{
				case WM_KILLFOCUS:
				{
					/*
					WindowFocusLostCallback callback = pWindowManager->GetFocusLostCallback();

					if (callback)
					{
						callback(*pWindow);
					}

					break;
					*/
				}
				case WM_SETFOCUS:
				{
					/*
					WindowFocusGainedCallback callback = pWindowManager->GetFocusGainedCallback();

					if (callback)
					{
						callback(*pWindow);
					}

					break;
					*/
				}
				case WM_SIZE:
				{
					/*
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
					*/
				}
				case WM_MOVE:
				{
					/*
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
					*/
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

		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
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
			return HGFX_NULL_HANDLE;
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
			return HGFX_NULL_HANDLE;
		}

		Bounds2i realBounds(adjustedPosX, adjustedPosY, adjustedWidth, adjustedHeight);
		Bounds2i clientBounds(posX, posY, posX + clientWidth, posY + clientHeight);

		Win32Window* pWindow = new Win32Window;
		pWindow->mHWND = windowHandle;
		pWindow->mTitle = title;
		pWindow->mBounds = realBounds;
		pWindow->mClientBounds = clientBounds;
		pWindow->mMaximized = false;
		pWindow->mMinimized = false;
		pWindow->mFullscreen = false;
		pWindow->mVisible = false;

		mHWNDMap.Put(reinterpret_cast<Handle64>(windowHandle), pWindow);

		SetWindowLongPtrW(windowHandle, GWLP_USERDATA, (LONG_PTR)this);

		return HVPWindow(pWindow);
	}

	void Win32Platform::DestroyWindow(HVPWindow window)
	{
		Win32Window* pWin32Window = static_cast<Win32Window*>(window);
		::DestroyWindow(pWin32Window->mHWND);
		delete pWin32Window;
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

	/*
	PlatformInput& Win32Platform::GetPlatformInput()
	{
		return mWin32PlatformInput;
	}

	PlatformTime& Win32Platform::GetPlatformTime()
	{
		return mWin32PlatformTime;
	}
	*/
}

