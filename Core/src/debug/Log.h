#pragma once
#include "platform\Console.h"

#include <stdio.h>

#define Log Quartz::Logger::Instance()

namespace Quartz
{
	class Logger
	{
	private:
		SystemConsole* mpLogConsole;
		FILE* mpLogFile;

		Logger();

	public:
		static Logger& Instance();

		void SetOutputConsole(SystemConsole* pConsole);
		void SetOutputFile(const char* filepath);

		void Raw(const ConsoleColor foreground, const ConsoleColor background, const char* format, ...);
		void RawV(const ConsoleColor foreground, const ConsoleColor background, const char* format, va_list args);
		void Custom(const char* type, const ConsoleColor foreground, 
			const ConsoleColor background, const char* format, ...);
		void CustomV(const char* type, const ConsoleColor foreground,
			const ConsoleColor background, const char* format, va_list args);

		void Debug(const char* format, ...);
		void DebugV(const char* format, va_list args);
		void Info(const char* format, ...);
		void InfoV(const char* format, va_list args);
		void Warning(const char* format, ...);
		void WarningV(const char* format, va_list args);
		void Error(const char* format, ...);
		void ErrorV(const char* format, va_list args);
		void Critical(const char* format, ...);
		void CriticalV(const char* format, va_list args);
	};
}