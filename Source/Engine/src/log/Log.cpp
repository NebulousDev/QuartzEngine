#include "Log.h"

#include <cstdio>
#include <cstdarg>
#include <time.h>

#define USE_MESSAGE_LOOP 0

namespace Quartz
{
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

	void LogUtil::Print(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatMessage(format, args));
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::Debug(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[DEBUG] " + FormatMessage(format, args) + "\n");
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::Info(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[INFO] " + FormatMessage(format, args) + "\n");
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::General(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[GENERAL] " + FormatMessage(format, args) + "\n");
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::Warning(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[WARNING] " + FormatMessage(format, args) + "\n");
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::Error(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[ERROR] " + FormatMessage(format, args) + "\n");
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::Critical(const char* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = StringAToStringW(FormatTime() + "[CRITICAL] " + FormatMessage(format, args) + "\n");
		va_end(args);

		wprintf_s(message.Str());
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

	void LogUtil::Print(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatMessageW(format, args);
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::Debug(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[DEBUG] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::Info(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[INFO] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::General(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[GENERAL] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::Warning(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[WARNING] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::Error(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[ERROR] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		wprintf_s(message.Str());
	}

	void LogUtil::Critical(const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);
		StringW message = FormatTimeW() + L"[CRITICAL] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

		wprintf_s(message.Str());
	}
}

