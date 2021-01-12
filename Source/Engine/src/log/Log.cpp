#include "Log.h"

#include <cstdio>
#include <cstdarg>
#include <time.h>

#include "util/String.h"
#include "../platform/Console.h"

#define USE_MESSAGE_LOOP 0

namespace Quartz
{
	VPDebugConsole* DebugLogger::pDebugConsole = nullptr;

	String FormatMessage(const char* format, va_list args)
	{
		va_list largs;
		va_copy(largs, args);

		Int32 length = vsnprintf(NULL, 0, format, largs);

		String message;
		message.Resize(length);

		vsnprintf(message.Data(), length + 1, format, args);

		return message;
	}

	String FormatTime()
	{
		char currentTime[26] = {};

		time_t timer;
		tm timeInfo;
		time(&timer);
		localtime_s(&timeInfo, &timer);
		strftime(currentTime, 26, "%H:%M:%S", &timeInfo);

		return "[" + String(currentTime) + "]";
	}

	void DebugLogger::SetDebugConsole(VPDebugConsole& console)
	{
		DebugLogger::pDebugConsole = &console;
	}

	void DebugLogger::Print(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatMessage(format, args));
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());

		//wprintf_s(message.Str());
	}

	void DebugLogger::Debug(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[DEBUG] " + FormatMessage(format, args) + "\n");
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	void DebugLogger::Info(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[INFO] " + FormatMessage(format, args) + "\n");
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_BLUE, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	void DebugLogger::General(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[GENERAL] " + FormatMessage(format, args) + "\n");
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	void DebugLogger::Warning(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[WARNING] " + FormatMessage(format, args) + "\n");
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	void DebugLogger::Error(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[ERROR] " + FormatMessage(format, args) + "\n");
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_RED, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	void DebugLogger::Critical(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[CRITICAL] " + FormatMessage(format, args) + "\n");
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_RED);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	/* WIDE */

	StringW FormatMessageW(const wchar_t* format, va_list args)
	{
		Int32 length = vswprintf(NULL, 0, format, args);

		StringW message;
		message.Resize(length);

		vswprintf(message.Data(), length + 1, format, args);

		return message;
	}

	StringW FormatTimeW()
	{
		wchar_t currentTime[26] = {};

		time_t timer;
		tm timeInfo;
		time(&timer);
		localtime_s(&timeInfo, &timer);
		wcsftime(currentTime, 26, L"%H:%M:%S", &timeInfo);

		return L"[" + StringW(currentTime) + L"]";
	}

	void DebugLogger::Print(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatMessageW(format, args);
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_DEFAULT, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());

		//wprintf_s(message.Str());
	}

	void DebugLogger::Debug(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[DEBUG] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	void DebugLogger::Info(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[INFO] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_BLUE, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	void DebugLogger::General(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[GENERAL] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	void DebugLogger::Warning(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[WARNING] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_YELLOW, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	void DebugLogger::Error(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[ERROR] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_RED, CONSOLE_COLOR_DEFAULT);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}

	void DebugLogger::Critical(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[CRITICAL] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		pDebugConsole->SetColor(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_RED);
		pDebugConsole->Print(message.Str());
		pDebugConsole->SetColor(CONSOLE_COLOR_LIGHT_GRAY, CONSOLE_COLOR_DEFAULT);

		//wprintf_s(message.Str());
	}
}

