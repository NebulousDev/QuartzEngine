#pragma once

#include "io/Console.h"
#include "Win32.h"

#include <stdio.h>

namespace Quartz
{
	class QUARTZ_API Win32SystemConsole : public SystemConsole
	{
	private:
		HANDLE mConsoleHandle;
		Int32 mCrtHandle;
		HWND mConsoleWindow;
		FILE* mpOutputStream;
		UInt16 mDefaultColor;

	public:
		void Init() override;
		void Destroy() override;
		void Show() override;
		void Hide() override;
		void SetTitle(const char* title) override;
		void SetColor(const ConsoleColor foreground, const ConsoleColor background) override;
		void Print(const char* text) override;
		void Printf(const char* format, ...) override;
		void PrintfV(const char* format, va_list args) override;
		void SetCursor(const Int16 posX, const Int16 posY) override;
		void Clear() override;
	};
}