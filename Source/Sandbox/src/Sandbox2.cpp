#include "Common.h"

#include "Engine.h"
#include "application/GameModule.h"

#include "Win32Platform.h"
#include "Win32VulkanGraphics.h"

#include "log/Log.h"

int main(int argc, char* argv[])
{
	using namespace Quartz;

	/*=====================================
		ENGINE INITIALIZATION
	=====================================*/

	/* Setup Platform */

	Platform* pPlatform	= Platform::CreateInstance(new Win32Platform());

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
	engineInfo.pGraphicsModule	= new Win32VulkanGraphics();

	pEngine->Initialize(engineInfo);
	pEngine->AddModule(new Game());

	pEngine->Start();

	/*=====================================
		GAME LOOP
	=====================================*/

	while (true)
	{
		pEngine->Tick();
	}
}