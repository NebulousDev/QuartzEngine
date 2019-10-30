#pragma once

#include "Common.h"
#include <stdarg.h>

namespace Quartz
{
	DLL_EXPORT enum ConsoleColor
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

	DLL_EXPORT class SystemConsole
	{
	public:
		virtual FORCE_INLINE void Init() = 0;
		virtual FORCE_INLINE void Destroy() = 0;
		virtual FORCE_INLINE void Show() = 0;
		virtual FORCE_INLINE void Hide() = 0;
		virtual FORCE_INLINE void SetTitle(const char* title) = 0;
		virtual FORCE_INLINE void SetColor(const ConsoleColor foreground, const ConsoleColor background) = 0;
		virtual FORCE_INLINE void Print(const char* text) = 0;
		virtual FORCE_INLINE void Printf(const char* format, ...) = 0;
		virtual FORCE_INLINE void PrintfV(const char* format, va_list) = 0;
		virtual FORCE_INLINE void SetCursor(const Int16 posX, const Int16 posY) = 0;
		virtual FORCE_INLINE void Clear() = 0;
	};
}