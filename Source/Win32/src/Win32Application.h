#pragma once

#include "platform/Application.h"
#include "util/Map.h"

#include "Win32.h"

namespace Quartz
{
	struct Win32ApplicationInfo
	{
		HINSTANCE hInstance;
		WNDCLASSW wndClass;
	};

	class Win32Window;

	class Win32Application : public Application
	{
	private:
		Win32ApplicationInfo mWin32Info;
		Map<Handle64, Win32Window*> mHWNDMap;

	public:
		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		Win32Application(const ApplicationInfo& info, const Win32ApplicationInfo& win32info);

		Window* CreateWindow(const WindowInfo& info) override;

		void PollMessages() override;
	};
}