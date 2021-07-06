#include "Win32Platform.h"
#include "Win32Window.h"

#include "Win32Application.h"
#include "Win32PeripheralController.h"

#include "Win32Util.h"
#include "log/Log.h"

#include <fcntl.h>
#include <stdio.h>
#include <io.h>

namespace Quartz
{
	Win32Platform::Win32Platform()
		: Platform({ L"Windows Platform" })
	{
		// Nothing
	}

	Bool8 Win32Platform::Init()
	{
		mpPeripheralController = new Win32PeripheralController();
		return true;
	}

	Bool8 Win32Platform::PostInit()
	{
		mpPeripheralController->RescanPeripherals();
		return true;
	}

	void Win32Platform::Shutdown()
	{
		delete mpPeripheralController;
	}

	void Win32Platform::Update(Float32 delta)
	{
		mpPeripheralController->PollInput();
	}

	DebugConsole* Win32Platform::CreateDebugConsole()
	{
		AllocConsole();

		HANDLE	std		= GetStdHandle(STD_OUTPUT_HANDLE);
		Int32	crt		= _open_osfhandle((intptr_t)std, _O_TEXT);
		FILE*	pStream = _fdopen(crt, "w");

		setvbuf(pStream, NULL, _IONBF, 1);
		_setmode(_fileno(pStream), _O_U16TEXT);

		CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
		GetConsoleScreenBufferInfo(std, &consoleInfo);

		WORD defaultColor = consoleInfo.wAttributes;

		HWND hwnd = GetConsoleWindow();

		Win32DebugConsoleInfo win32ConsoleInfo;
		win32ConsoleInfo.hwnd			= hwnd;
		win32ConsoleInfo.handle			= std;
		win32ConsoleInfo.crt			= crt;
		win32ConsoleInfo.pOutputStream	= pStream;
		win32ConsoleInfo.defaultColor	= defaultColor;

		return new Win32DebugConsole(win32ConsoleInfo);
	}

	void Win32Platform::DestroyDebugConsole(DebugConsole* pDebugConsole)
	{
		FreeConsole();
		delete pDebugConsole;
	}

	Application* Win32Platform::CreateApplication(const ApplicationInfo& info)
	{
		WNDCLASSW wndClass = {};
		wndClass.lpfnWndProc	= Win32Application::WindowProc;
		wndClass.hInstance		= GetModuleHandle(NULL);
		wndClass.lpszClassName	= info.name.Str();

		ATOM result = RegisterClassW(&wndClass);

		if (result == 0)
		{
			Log::Critical(L"Unable to create application '%s'! RegisterClass() failed.", info.name.Str());
			PrintLastError();
			return nullptr;
		}

		Win32ApplicationInfo win32info;
		win32info.hInstance = GetModuleHandle(NULL);
		win32info.wndClass	= wndClass;

		return new Win32Application(info, win32info);
	}

	Bool8 Win32Platform::DestroyApplication(Application* application)
	{
		return Bool8();
	}

	Time* Win32Platform::GetTime()
	{
		return &mTime;
	}
}

