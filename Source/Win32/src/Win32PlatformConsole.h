#pragma once

#include "platform/VPDebugConsole.h"
#include "Win32.h"

#include <stdio.h>

namespace Quartz
{
	struct Win32DebugConsoleInfo
	{
		HANDLE	handle;
		Int32	crt;
		HWND	hwnd;
		FILE*	pOutputStream;
		WORD	defaultColor;
	};

	class QUARTZ_API Win32DebugConsole : public DebugConsole
	{
	private:
		Win32DebugConsoleInfo mWin32ConsoleInfo;

	public:
		Win32DebugConsole(const Win32DebugConsoleInfo& win32ConsoleInfo);

		void Show() override;
		void Hide() override;
		void SetTitle(const wchar_t* title) override;
		void SetColor(const TextColor foreground, const TextColor background) override;
		void Print(const wchar_t* text) override;
		void SetCursor(const Int16 posX, const Int16 posY) override;
		void Clear() override;
	};
}