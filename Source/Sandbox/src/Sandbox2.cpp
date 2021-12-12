#include "Common.h"

#include "Engine.h"
#include "application/GameModule.h"

#include "Win32Platform.h"
#include "Win32VulkanGraphics.h"

#include "log/Log.h"

#include "util/TypeId.h"

int main(int argc, char* argv[])
{
	using namespace Quartz;

	/*=====================================
		ENGINE INITIALIZATION
	=====================================*/

	/* Setup Modules */

	Platform*	pPlatform	= new Win32Platform();
	Graphics*	pGraphics	= new Win32VulkanGraphics();
	Game*		pGame		= new Game();

	/* Setup Logging */

	DebugConsole* pConsole = pPlatform->CreateDebugConsole();
	Log::SetDebugConsole(pConsole);

	/* Print Console Splash */

	pConsole->SetColor(TEXT_COLOR_WHITE, TEXT_COLOR_DEFAULT);
	pConsole->Print(L"-------------------------------------------------------\n");
	pConsole->Print(L"|                    Nebulous Games                   |\n");
	pConsole->Print(L"|                 QUARTZ ENGINE v0.4.0                |\n");
	pConsole->Print(L"-------------------------------------------------------\n");
	pConsole->Print(L" ~ Copyright © Ben Ratcliff (NebulousDev) 2019-2021 ~\n\n");
	pConsole->SetColor(TEXT_COLOR_DEFAULT, TEXT_COLOR_DEFAULT);

	/* Create Engine */

	Engine* pEngine = Engine::GetInstance();

	EngineInfo engineInfo;
	engineInfo.gameInfo.name	= L"Sandbox";
	engineInfo.gameInfo.version = L"1.0.0";
	engineInfo.pGraphicsModule	= pGraphics;
	engineInfo.pPlatformModule	= pPlatform;
	engineInfo.targetTPS		= 60.0f;

	pEngine->Initialize(engineInfo);
	pEngine->AddModule(pGame);
	pEngine->Start();

	/* Shutdown */

	delete pPlatform;
	delete pGraphics;
	delete pGame;
}