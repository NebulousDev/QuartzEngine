#pragma once

#include "Common.h"
#include <stdarg.h>

namespace Quartz
{
	enum TextColor
	{
		TEXT_COLOR_DEFAULT,
		TEXT_COLOR_RED,
		TEXT_COLOR_LIGHT_RED,
		TEXT_COLOR_GOLD,
		TEXT_COLOR_YELLOW,
		TEXT_COLOR_GREEN,
		TEXT_COLOR_LIGHT_GREEN,
		TEXT_COLOR_BLUE,
		TEXT_COLOR_LIGHT_BLUE,
		TEXT_COLOR_CYAN,
		TEXT_COLOR_DARK_CYAN,
		TEXT_COLOR_MAGENTA,
		TEXT_COLOR_DARK_MAGENTA,
		TEXT_COLOR_GRAY,
		TEXT_COLOR_LIGHT_GRAY,
		TEXT_COLOR_WHITE,
		TEXT_COLOR_BLACK
	};

	class QUARTZ_API DebugConsole
	{
	public:
		virtual void Show() = 0;
		virtual void Hide() = 0;
		virtual void SetTitle(const wchar_t* title) = 0;
		virtual void SetColor(const TextColor foreground, const TextColor background) = 0;
		virtual void Print(const wchar_t* text) = 0;
		virtual void SetCursor(const Int16 posX, const Int16 posY) = 0;
		virtual void Clear() = 0;
	};
}