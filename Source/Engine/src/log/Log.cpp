#include "Log.h"

#include <cstdio>
#include <cstdarg>
#include <time.h>

#include "../Engine.h"

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
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_NONE;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = StringAToStringW(FormatMessage(format, args));
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::Debug(const char* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_DEBUG;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = StringAToStringW(FormatTime() + "[DEBUG] " + FormatMessage(format, args) + "\n");
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::Info(const char* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_INFO;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = StringAToStringW(FormatTime() + "[INFO] " + FormatMessage(format, args) + "\n");
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::General(const char* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_GENERAL;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = StringAToStringW(FormatTime() + "[GENERAL] " + FormatMessage(format, args) + "\n");
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::Warning(const char* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_WARNING;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = StringAToStringW(FormatTime() + "[WARNING] " + FormatMessage(format, args) + "\n");
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::Error(const char* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_ERROR;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = StringAToStringW(FormatTime() + "[ERROR] " + FormatMessage(format, args) + "\n");
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::Critical(const char* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_CRITICAL;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = StringAToStringW(FormatTime() + "[CRITICAL] " + FormatMessage(format, args) + "\n");
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
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
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_NONE;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = FormatMessageW(format, args);
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::Debug(const wchar_t* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_DEBUG;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = FormatTimeW() + L"[DEBUG] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::Info(const wchar_t* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_INFO;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = FormatTimeW() + L"[INFO] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::General(const wchar_t* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_GENERAL;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = FormatTimeW() + L"[GENERAL] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::Warning(const wchar_t* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_WARNING;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = FormatTimeW() + L"[WARNING] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::Error(const wchar_t* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_ERROR;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = FormatTimeW() + L"[ERROR] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}

	void LogUtil::Critical(const wchar_t* format, ...)
	{
		Engine& engine = Engine::GetInstanceHandle();

		DebugMessageEvent debugMessageEvent;
		debugMessageEvent.category = MESSAGE_CRITICAL;

		va_list args;
		va_start(args, format);
		debugMessageEvent.message = FormatTimeW() + L"[CRITICAL] " + FormatMessageW(format, args) + L"\n";
		va_end(args);

#if USE_MESSAGE_LOOP
		engine.GetEventSystem().Publish(EVENT_PRIORITY_IMMEDIATE, debugMessageEvent);
#else
		wprintf_s(debugMessageEvent.message.Str());
#endif
	}
}

