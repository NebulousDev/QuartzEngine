#pragma once

#include "Common.h"
#include <stdarg.h>

namespace Quartz
{
	enum QUARTZ_API ConsoleColor
	{
		COLOR_DEFAULT,
		COLOR_RED,
		COLOR_LIGHT_RED,
		COLOR_GOLD,
		COLOR_YELLOW,
		COLOR_GREEN,
		COLOR_LIGHT_GREEN,
		COLOR_BLUE,
		COLOR_LIGHT_BLUE,
		COLOR_CYAN,
		COLOR_DARK_CYAN,
		COLOR_MAGENTA,
		COLOR_DARK_MAGENTA,
		COLOR_GRAY,
		COLOR_LIGHT_GRAY,
		COLOR_WHITE,
		COLOR_BLACK
	};

	class QUARTZ_API PlatformConsole
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