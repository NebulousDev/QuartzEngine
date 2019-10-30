#pragma once

#include "io/Console.h"
#include "Win32.h"

#include <stdio.h>

namespace Quartz
{
	__declspec(dllexport) class Win32SystemConsole : public SystemConsole
	{
	private:
		HANDLE mConsoleHandle;
		Int32 mCrtHandle;
		HWND mConsoleWindow;
		FILE* mpOutputStream;
		UInt16 mDefaultColor;

	public:
		__declspec(dllexport) void Init() override;
		__declspec(dllexport) void Destroy() override;
		__declspec(dllexport) void Show() override;
		__declspec(dllexport) void Hide() override;
		__declspec(dllexport) void SetTitle(const char* title) override;
		__declspec(dllexport) void SetColor(const ConsoleColor foreground, const ConsoleColor background) override;
		__declspec(dllexport) void Print(const char* text) override;
		__declspec(dllexport) void Printf(const char* format, ...) override;
		__declspec(dllexport) void PrintfV(const char* format, va_list args) override;
		__declspec(dllexport) void SetCursor(const Int16 posX, const Int16 posY) override;
		__declspec(dllexport) void Clear() override;
	};
}