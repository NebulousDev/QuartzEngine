#include "Win32Console.h"
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

	void Win32SystemConsole::Init()
	{
		AllocConsole();

		mConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		mCrtHandle = _open_osfhandle((UInt64)mConsoleHandle, _O_TEXT);
		mpOutputStream = _fdopen(mCrtHandle, "w");
		setvbuf(mpOutputStream, NULL, _IONBF, 1);

		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(mConsoleHandle, &consoleInfo);
		mDefaultColor = consoleInfo.wAttributes;

		mConsoleWindow = GetConsoleWindow();

		// https://support.microsoft.com/en-us/help/190351/how-to-spawn-console-processes-with-redirected-standard-handles

		/*
		STARTUPINFO startInfo = {};
		startInfo.cb = sizeof(STARTUPINFO);
		startInfo.wShowWindow = SW_HIDE;
		startInfo.dwFlags = STARTF_USESHOWWINDOW;

		SECURITY_ATTRIBUTES securityAttribs = {};
		securityAttribs.nLength = sizeof(SECURITY_ATTRIBUTES);
		securityAttribs.lpSecurityDescriptor = NULL;
		securityAttribs.bInheritHandle = TRUE;

		HANDLE tempOutputRead, outputRead, outputWrite;
		HANDLE tempInputWrite, inputRead, inputWrite;
		HANDLE errorWrite;
		HANDLE thread;
		DWORD thredID;

		if (!CreatePipe(&tempOutputRead, &outputWrite, &securityAttribs, 0))
			printf("Failed to create outputWrite pipe!");

		if (!DuplicateHandle(GetCurrentProcess(), outputWrite,
			GetCurrentProcess(), &errorWrite, 0, TRUE, DUPLICATE_SAME_ACCESS))
			printf("Failed to create errorWrite pipe!");

		if (!CreatePipe(&inputRead, &tempInputWrite, &securityAttribs, 0))
			printf("Failed to create inputRead pipe!");

		if (!DuplicateHandle(GetCurrentProcess(), tempOutputRead,
			GetCurrentProcess(), &outputRead, 0, FALSE, DUPLICATE_SAME_ACCESS))
			printf("Failed to create outputRead pipe!");

		if (!DuplicateHandle(GetCurrentProcess(), tempInputWrite,
			GetCurrentProcess(), &inputWrite, 0, FALSE, DUPLICATE_SAME_ACCESS))
			printf("Failed to create inputWrite pipe!");

		if (!CloseHandle(tempOutputRead)) 
			printf("Failed to close tempOutputRead pipe!");

		if (!CloseHandle(tempInputWrite))
			printf("Failed to close tempInputWrite pipe!");

		PROCESS_INFORMATION processInfo = {};

		BOOL result = CreateProcess(
			NULL,
			"-i",
			NULL,
			NULL,
			TRUE,
			CREATE_NEW_CONSOLE,
			NULL,
			NULL,
			&startInfo,
			&processInfo);
		*/
	}

	void Win32SystemConsole::Destroy()
	{
		//CloseHandle((HANDLE)mCrtHandle);
		//FreeConsole();
	}

	void Win32SystemConsole::Show()
	{
		ShowWindow(mConsoleWindow, SW_SHOW);
	}

	void Win32SystemConsole::Hide()
	{
		ShowWindow(mConsoleWindow, SW_HIDE);
	}

	void Win32SystemConsole::SetTitle(const char* title)
	{
		SetConsoleTitle(title);
	}

	void Win32SystemConsole::SetColor(const ConsoleColor foreground, const ConsoleColor background)
	{
		WORD forgroundColor = !foreground ? mDefaultColor & 0x0F : sWin32Colors[foreground];
		WORD backgroundColor = !foreground ? mDefaultColor & 0xF0 : (sWin32Colors[background] << 4) & 0xF0;
		SetConsoleTextAttribute(mConsoleHandle, forgroundColor | backgroundColor);
	}

	void Win32SystemConsole::Print(const char* text)
	{
		Printf(text);
	}

	void Win32SystemConsole::Printf(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		vfprintf_s(mpOutputStream, format, args);
		va_end(args);
	}

	void Win32SystemConsole::PrintfV(const char* format, va_list args)
	{
		vfprintf_s(mpOutputStream, format, args);
	}

	void Win32SystemConsole::SetCursor(const Int16 posX, const Int16 posY)
	{
		SetConsoleCursorPosition(mConsoleHandle, { posX, posY });
	}

	void Win32SystemConsole::Clear()
	{
		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(mConsoleHandle, &consoleInfo);
		UInt32 consoleLength = consoleInfo.dwSize.X * consoleInfo.dwSize.Y;
		DWORD filled = 0;

		FillConsoleOutputCharacter(mConsoleHandle, ' ', consoleLength, { 0, 0 }, &filled);
		FillConsoleOutputAttribute(mConsoleHandle, mDefaultColor, consoleLength, { 0, 0 }, &filled);

		SetCursor(0, 0);
	}
}