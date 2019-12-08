#pragma once

#include "Common.h"
#include "util\Array.h"
#include "util\String.h"

#include "io\Log.h"
#include "platform\Window.h"
#include "platform\System.h"

#include "io\Win32Console.h"
#include "platform\Win32Window.h"
#include "platform\Win32System.h"

#include "Engine.h"

#include "VulkanTest.h"

#include <iostream>

#include "Win32.h"

using namespace Quartz;

int main()
{
	/* Initialize System */
	System::Init(new Win32System());
	Log.Info("System Initialized Successfully.");

	/* Initialize Console */
	SystemConsole* pConsole = System::CreateConsole();
	pConsole->Init();
	Log.SetOutputConsole(pConsole);
	Log.Info("Log Initialized Successfully.");

	/* Create Window */
	Window* pWindow = System::CreateWindow(0, 0, 640, 480, "Quartz Engine - Sandbox");
	pWindow->Show();
	Log.Info("Window Created Successfully.");

	/* Start Engine */
	Log.Info("Starting Engine...");
	Engine& engine = Engine::GetInstance();

	/* Game Loop*/
	Time64 lastTime = 0.0;
	Time64 currentTime = 0.0; 
	Time64 deltaTime = 0.0;
	Time64 elapsedTime = 0.0;

	UInt32 fps = 0;

	currentTime = System::GetTimeNanoseconds();

	while (true)
	{
		engine.Update();
		pWindow->Update();

		lastTime = currentTime;
		currentTime = System::GetTimeNanoseconds();
		deltaTime = currentTime - lastTime;
		elapsedTime += deltaTime;

		if (elapsedTime >= 1.0)
		{
			Log.Info("MS: %.3llfms, FPS: %d", deltaTime * 1000, fps);
			elapsedTime = 0.0;
			fps = 0;
		}

		fps++;
	}

	/* Shutdown */
	Log.Info("Engine shutdown complete.");

	return 0;
}