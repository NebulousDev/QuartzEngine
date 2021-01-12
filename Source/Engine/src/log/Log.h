#pragma once

#include "util\String.h"

namespace Quartz
{
	class VPDebugConsole;

	class QUARTZ_API DebugLogger
	{
	private:
		static VPDebugConsole* pDebugConsole;

	public:
		static void SetDebugConsole(VPDebugConsole& console);

		static void Print(const char* format, ...);
		static void Debug(const char* format, ...);
		static void Info(const char* format, ...);
		static void General(const char* format, ...);
		static void Warning(const char* format, ...);
		static void Error(const char* format, ...);
		static void Critical(const char* format, ...);

		static void Print(const wchar_t* format, ...);
		static void Debug(const wchar_t* format, ...);
		static void Info(const wchar_t* format, ...);
		static void General(const wchar_t* format, ...);
		static void Warning(const wchar_t* format, ...);
		static void Error(const wchar_t* format, ...);
		static void Critical(const wchar_t* format, ...);
	};

	#define Log DebugLogger
}