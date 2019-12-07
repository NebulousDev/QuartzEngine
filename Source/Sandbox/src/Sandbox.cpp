#pragma once

#include "Common.h"
#include "util\Array.h"
#include "util\String.h"

#include "io\Log.h"
#include "interface\Window.h"

#include "Win32Console.h"
#include "interface\Win32Window.h"

#include "Engine.h"

#include "VulkanTest.h"

#include <iostream>

using namespace Quartz;

int main()
{
	/* Initialize Console */
	// Eventually move to PlatformLayer->CreateConsole() etc...
	SystemConsole* pConsole = new Win32SystemConsole();
	pConsole->Init();
	Log.SetOutputConsole(pConsole);
	Log.Info("Log System Initialized Successfully.");

	/* Create Window */
	// Eventually move to PlatformLayer->CreateWindow() etc...
	Window* pWindow = new Win32Window(0, 0, 640, 480, "Quartz Engine - Sandbox");
	pWindow->Show();
	Log.Info("Window Created Successfully.");

	/* Start Engine */
	Log.Info("Starting Engine...");
	Engine& engine = Engine::GetInstance();
	engine.Start();

	system("PAUSE");

	return 0;
}