#pragma once

#include "Types.h"
#include "util\Array.h"
#include "util\String.h"

#include "io\Log.h"

#include "Win32Console.h"

using namespace Quartz;

int main()
{
	/* Initialize Console */
	SystemConsole* pConsole = new Win32SystemConsole();
	pConsole->Init();
	Log.SetOutputConsole(pConsole);

	Log.Info("Log System Initialized...");

	return 0;
}