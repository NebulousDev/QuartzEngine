
#include "Win32.h"
#include "Win32Util.h"
#include "log/Log.h"

namespace Quartz
{
	void PrintLastError()
	{
		DWORD error = GetLastError();

		LPWSTR pErrorMessage = nullptr;

		FormatMessageW(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			error,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPWSTR)&pErrorMessage,
			0, NULL);

		if (pErrorMessage == nullptr)
		{
			// No error
			return;
		}

		Log::Error(pErrorMessage);

		LocalFree(pErrorMessage);
	}
}