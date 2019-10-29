#include "Log.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include <stdio.h>
#include <time.h>

namespace Quartz
{
	Logger::Logger() { }

	Logger& Logger::Instance()
	{
		static Logger sLogManager;
		return sLogManager;
	}

	void Logger::SetOutputConsole(SystemConsole* pConsole)
	{
		mpLogConsole = pConsole;
	}

	void Logger::SetOutputFile(const char* filepath)
	{
		// TODO
	}

	void Logger::Raw(const ConsoleColor foreground, const ConsoleColor background, const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		if (mpLogConsole)
		{
			mpLogConsole->SetColor(foreground, background);
			mpLogConsole->PrintfV(format, args);
			mpLogConsole->SetColor(COLOR_DEFAULT, COLOR_DEFAULT);
		}
		va_end(args);
	}

	void Logger::RawV(const ConsoleColor foreground, const ConsoleColor background, const char* format, va_list args)
	{
		if (mpLogConsole)
		{
			mpLogConsole->SetColor(foreground, background);
			mpLogConsole->PrintfV(format, args);
			mpLogConsole->SetColor(COLOR_DEFAULT, COLOR_DEFAULT);
		}
	}

	void Logger::Custom(const char* type, const ConsoleColor foreground, const ConsoleColor background, const char* format, ...)
	{
		static char result[2048] = {};
		static char currentTime[26];

		time_t timer;
		struct tm* timeInfo;
		time(&timer);
		timeInfo = localtime(&timer);
		strftime(currentTime, 26, "%H:%M:%S", timeInfo);

		strcpy_s(result, "[");
		strcat_s(result, currentTime);
		strcat_s(result, "] [");
		strcat_s(result, type);
		strcat_s(result, "]: ");
		strcat_s(result, format);
		strcat_s(result, "\n");

		va_list args;
		va_start(args, format);
		if (mpLogConsole)
		{
			mpLogConsole->SetColor(foreground, background);
			mpLogConsole->PrintfV(result, args);
			mpLogConsole->SetColor(COLOR_DEFAULT, COLOR_DEFAULT);
		}
		//if(mpLogFile)
			//vfprintf(mpLogFile, result, args);
		va_end(args);
	}

	void Logger::CustomV(const char* type, const ConsoleColor foreground, const ConsoleColor background, const char* format, va_list args)
	{
		static char result[2048] = {};
		static char currentTime[26];

		time_t timer;
		struct tm* timeInfo;
		time(&timer);
		timeInfo = localtime(&timer);
		strftime(currentTime, 26, "%H:%M:%S", timeInfo);

		strcpy_s(result, "[");
		strcat_s(result, currentTime);
		strcat_s(result, "] [");
		strcat_s(result, type);
		strcat_s(result, "]: ");
		strcat_s(result, format);
		strcat_s(result, "\n");

		if (mpLogConsole)
		{
			mpLogConsole->SetColor(foreground, background);
			mpLogConsole->PrintfV(result, args);
			mpLogConsole->SetColor(COLOR_DEFAULT, COLOR_DEFAULT);
		}
		//if (mpLogFile)
			//vfprintf(mpLogFile, result, args);
	}

	void Logger::Debug(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		CustomV("Debug", COLOR_LIGHT_GRAY, COLOR_DEFAULT, format, args);
		va_end(args);
	}

	void Logger::DebugV(const char* format, va_list args)
	{
		CustomV("Debug", COLOR_LIGHT_GRAY, COLOR_DEFAULT, format, args);
	}

	void Logger::Info(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		CustomV("Info", COLOR_DEFAULT, COLOR_DEFAULT, format, args);
		va_end(args);
	}

	void Logger::InfoV(const char* format, va_list args)
	{
		CustomV("Info", COLOR_WHITE, COLOR_DEFAULT, format, args);
	}

	void Logger::Warning(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		CustomV("Warning", COLOR_YELLOW, COLOR_DEFAULT, format, args);
		va_end(args);
	}

	void Logger::WarningV(const char* format, va_list args)
	{
		CustomV("Warning", COLOR_YELLOW, COLOR_DEFAULT, format, args);
	}

	void Logger::Error(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		CustomV("Error", COLOR_LIGHT_RED, COLOR_DEFAULT, format, args);
		va_end(args);
	}

	void Logger::ErrorV(const char * format, va_list args)
	{
		CustomV("Error", COLOR_LIGHT_RED, COLOR_DEFAULT, format, args);
	}

	void Logger::Critical(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		CustomV("Critical", COLOR_WHITE, COLOR_RED, format, args);
		va_end(args);
	}

	void Logger::CriticalV(const char* format, va_list args)
	{
		CustomV("Critical", COLOR_WHITE, COLOR_RED, format, args);
	}
}