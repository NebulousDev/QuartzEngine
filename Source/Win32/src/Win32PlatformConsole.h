#pragma once

#include "platform/Console.h"
#include "Win32.h"

#include <stdio.h>

namespace Quartz
{
	class QUARTZ_API Win32Console : public VPDebugConsole
	{
	private:
		HANDLE mConsoleHandle;
		Int32 mCrtHandle;
		HWND mConsoleWindow;
		FILE* mpOutputStream;
		UInt16 mDefaultColor;

	public:
		void Create() override;
		void Destroy() override;
		void Show() override;
		void Hide() override;
		void SetTitle(const wchar_t* title) override;
		void SetColor(const ConsoleColor foreground, const ConsoleColor background) override;
		void Print(const wchar_t* text) override;
		void SetCursor(const Int16 posX, const Int16 posY) override;
		void Clear() override;
	};
}