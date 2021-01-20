#include "Engine.h"

#include "log/Log.h"

namespace Quartz
{
	Engine* Engine::sInstance;

	void Engine::Setup(const EngineInfo& info)
	{
		mGameInfo = info.gameInfo;

		mpPlatform = info.pPlatform;
		mpGraphics = info.pGraphics;

		mpInput = new Input();
		mpWindowManager = new WindowManager();

		SetupDebugConsole(info.showDebugConsole);
		SetupDebugLogging();

		PrintSplash();
		Log::General(L"Game Version: %s %s", mGameInfo.name.Str(), mGameInfo.version.Str());
		Log::Info("Engine is starting...");

		mpPlatform->PreInitialize();
		mpGraphics->PreInitialize();
		mpWindowManager->PreInitialize();
		mpInput->PreInitialize();

		mpPlatform->Initialize();
		mpGraphics->Initialize();
		mpWindowManager->Initialize();
		mpInput->Initialize();
	}

	void Engine::SetupDebugConsole(Bool8 showConsole)
	{
		mpDebugConsole = mpPlatform->CreateDebugConsole();
		mpDebugConsole->Create();

		StringW consoleTitle = mGameInfo.name + L" - Debug Console";

		mpDebugConsole->SetTitle(consoleTitle.Str());

		if (showConsole)
		{
			mpDebugConsole->Show();
		}
	}

	void Engine::SetupDebugLogging()
	{
		DebugLogger::SetDebugConsole(*mpDebugConsole);
	}

	void Engine::PrintSplash()
	{
		mpDebugConsole->SetColor(CONSOLE_COLOR_WHITE, CONSOLE_COLOR_DEFAULT);
		mpDebugConsole->Print(L"-------------------------------------------------------\n");
		mpDebugConsole->Print(L"|                    Nebulous Games                   |\n");
		mpDebugConsole->Print(L"|                 QUARTZ ENGINE v0.3.0                |\n");
		mpDebugConsole->Print(L"-------------------------------------------------------\n");
		mpDebugConsole->Print(L" ~ Copyright © Ben Ratcliff (NebulousDev) 2019-2021 ~\n\n");
		mpDebugConsole->SetColor(CONSOLE_COLOR_DEFAULT, CONSOLE_COLOR_DEFAULT);
	}

	Engine& Engine::GetInstance()
	{
		return *sInstance;
	}

	Engine& Engine::CreateInstance(const EngineInfo& info)
	{
		if (!sInstance)
		{
			sInstance = new Engine();
			sInstance->Setup(info);
		}
		else
		{
			Log::Warning("Attempted to create a new instance of Engine when one already exists.");
		}

		return *sInstance;
	}

	void Engine::Start()
	{
		Log::Info("Engine Started.");
	}

	const GameInfo& Engine::GetGameInfo()
	{
		return mGameInfo;
	}

	VPPlatform& Engine::GetPlatform()
	{
		return *mpPlatform;
	}

	VGFXContext& Engine::GetGraphics()
	{
		return *mpGraphics;
	}

	Input& Engine::GetInput()
	{
		return *mpInput;
	}

	WindowManager& Engine::GetWindowManager()
	{
		return *mpWindowManager;
	}
}

