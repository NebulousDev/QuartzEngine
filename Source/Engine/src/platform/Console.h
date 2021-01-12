#pragma once

#include "Common.h"
#include <stdarg.h>

namespace Quartz
{
	enum ConsoleColor
	{
		CONSOLE_COLOR_DEFAULT,
		CONSOLE_COLOR_RED,
		CONSOLE_COLOR_LIGHT_RED,
		CONSOLE_COLOR_GOLD,
		CONSOLE_COLOR_YELLOW,
		CONSOLE_COLOR_GREEN,
		CONSOLE_COLOR_LIGHT_GREEN,
		CONSOLE_COLOR_BLUE,
		CONSOLE_COLOR_LIGHT_BLUE,
		CONSOLE_COLOR_CYAN,
		CONSOLE_COLOR_DARK_CYAN,
		CONSOLE_COLOR_MAGENTA,
		CONSOLE_COLOR_DARK_MAGENTA,
		CONSOLE_COLOR_GRAY,
		CONSOLE_COLOR_LIGHT_GRAY,
		CONSOLE_COLOR_WHITE,
		CONSOLE_COLOR_BLACK
	};

	class QUARTZ_API VPDebugConsole
	{
	public:
		virtual void Create() = 0;
		virtual void Destroy() = 0;
		virtual void Show() = 0;
		virtual void Hide() = 0;
		virtual void SetTitle(const wchar_t* title) = 0;
		virtual void SetColor(const ConsoleColor foreground, const ConsoleColor background) = 0;
		virtual void Print(const wchar_t* text) = 0;
		virtual void SetCursor(const Int16 posX, const Int16 posY) = 0;
		virtual void Clear() = 0;
	};
}