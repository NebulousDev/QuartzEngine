#pragma once

#include "util\Singleton.h"
#include "util\String.h"

namespace Quartz
{
	class QUARTZ_API LogUtil : public Singleton<LogUtil>
	{
	public:
		friend class Singleton<LogUtil>;

	private:
		LogUtil() = default;

	public:
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

	#define Log LogUtil::GetInstanceHandle()
}