#include "Win32PlatformConsole.h"

#include <fcntl.h>
#include <stdio.h>
#include <io.h>

#include "Win32.h"

namespace Quartz
{
	WORD sWin32Colors[] = 
	{
		/* WIN32_COLOR_NONE */			0,
		/* WIN32_COLOR_RED */			FOREGROUND_RED,
		/* WIN32_COLOR_LIGHT_RED */		FOREGROUND_RED | FOREGROUND_INTENSITY,
		/* WIN32_COLOR_GOLD */			FOREGROUND_RED | FOREGROUND_GREEN,
		/* WIN32_COLOR_YELLOW */		FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		/* WIN32_COLOR_GREEN */			FOREGROUND_GREEN,
		/* WIN32_COLOR_LIGHT_GREEN */	FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		/* WIN32_COLOR_BLUE */			FOREGROUND_BLUE,
		/* WIN32_COLOR_LIGHT_BLUE */	FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		/* WIN32_COLOR_CYAN */			FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		/* WIN32_COLOR_DARK_CYAN */		FOREGROUND_GREEN | FOREGROUND_BLUE,
		/* WIN32_COLOR_MAGENTA */		FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
		/* WIN32_COLOR_DARK_MAGENTA	*/	FOREGROUND_RED | FOREGROUND_BLUE,
		/* WIN32_COLOR_GRAY	*/			0x8,
		/* WIN32_COLOR_LIGHT_GRAY */	0x7,
		/* WIN32_COLOR_WHITE */			FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
		/* WIN32_COLOR_BLACK */			0x0
	};

	Win32DebugConsole::Win32DebugConsole(const Win32DebugConsoleInfo& win32ConsoleInfo)
		: mWin32ConsoleInfo(win32ConsoleInfo)
	{
		// Nothing
	}

	void Win32DebugConsole::Show()
	{
		ShowWindow(mWin32ConsoleInfo.hwnd, SW_SHOW);
	}

	void Win32DebugConsole::Hide()
	{
		ShowWindow(mWin32ConsoleInfo.hwnd, SW_HIDE);
	}

	void Win32DebugConsole::SetTitle(const wchar_t* title)
	{
		SetConsoleTitleW(title);
	}

	void Win32DebugConsole::SetColor(const TextColor foreground, const TextColor background)
	{
		WORD forgroundColor = !foreground ? mWin32ConsoleInfo.defaultColor & 0x0F : sWin32Colors[foreground];
		WORD backgroundColor = !foreground ? mWin32ConsoleInfo.defaultColor & 0xF0 : (sWin32Colors[background] << 4) & 0xF0;
		SetConsoleTextAttribute(mWin32ConsoleInfo.handle, forgroundColor | backgroundColor);
	}

	void Win32DebugConsole::Print(const wchar_t* text)
	{
		fwprintf_s(mWin32ConsoleInfo.pOutputStream, text);
	}

	void Win32DebugConsole::SetCursor(const Int16 posX, const Int16 posY)
	{
		SetConsoleCursorPosition(mWin32ConsoleInfo.handle, { posX, posY });
	}

	void Win32DebugConsole::Clear()
	{
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(mWin32ConsoleInfo.handle, &consoleInfo);
		UInt32 consoleLength = consoleInfo.dwSize.X * consoleInfo.dwSize.Y;
		DWORD filled = 0;

		FillConsoleOutputCharacter(mWin32ConsoleInfo.handle, ' ', consoleLength, { 0, 0 }, &filled);
		FillConsoleOutputAttribute(mWin32ConsoleInfo.handle, mWin32ConsoleInfo.defaultColor, consoleLength, { 0, 0 }, &filled);

		SetCursor(0, 0);
	}
}