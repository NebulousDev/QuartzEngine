#include "Engine.h"

#include "log/Log.h"

namespace Quartz
{
	GameInfo Engine::mGameInfo{};
	VPPlatform* Engine::mpPlatform = nullptr;
	VGFXContext* Engine::mpGraphics = nullptr;
	VPDebugConsole* Engine::mpDebugConsole = nullptr;

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

	void Engine::Setup(const EngineInfo& info)
	{
		mGameInfo = info.gameInfo;

		mpPlatform = info.pPlatform;
		mpGraphics = info.pGraphics;

		SetupDebugConsole(info.showDebugConsole);
		SetupDebugLogging();

		PrintSplash();
		Log::General(L"Game Version: %s %s", mGameInfo.name.Str(), mGameInfo.version.Str());
		Log::Info("Engine is starting...");

		mpPlatform->PreInitialize();
		mpGraphics->PreInitialize();

		mpPlatform->Initialize();
		mpGraphics->Initialize();
	}

	void Engine::Start()
	{
		
	}

	const GameInfo& Engine::GetGameInfo()
	{
		return mGameInfo;
	}

	VPPlatform& Engine::GetPlatformContext()
	{
		return *mpPlatform;
	}

	VGFXContext& Engine::GetGraphicsContext()
	{
		return *mpGraphics;
	}
}

