#pragma once

#include "Types.h"
#include "util\Array.h"
#include "util\String.h"

#include "io\Log.h"

#include "Win32Console.h"

int main()
{
	Quartz::Array<Quartz::String> strings;

	strings.PushBack("Hello");
	strings.PushBack("World!");

	Quartz::SystemConsole* pConsole = new Quartz::Win32SystemConsole();
	pConsole->Init();
	pConsole->Show();

	Log.SetOutputConsole(pConsole);

	Log.Critical("%s\n", (strings[0] + " " + strings[1]).Str());

	return 0;
}