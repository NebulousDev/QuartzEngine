#pragma once

#include "Common.h"
#include "util\Array.h"
#include "util\String.h"

#include "io\Log.h"

#include "Win32Console.h"

#include "VulkanTest.h"

#include <iostream>

using namespace Quartz;

int main()
{
	/* Initialize Console */
	SystemConsole* pConsole = new Win32SystemConsole();
	pConsole->Init();
	Log.SetOutputConsole(pConsole);

	Log.Info("Log System Initialized...");

	/* Test Vulkan */
	TestVulkan();

	Array<int> ints = { 1, 2, 3, 4, 5 };

	system("PAUSE");

	return 0;
}