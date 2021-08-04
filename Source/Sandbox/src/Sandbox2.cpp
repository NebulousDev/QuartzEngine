#include "Common.h"

#include "Engine.h"
#include "application/GameModule.h"

#include "Win32Platform.h"
#include "Win32VulkanGraphics.h"

#include "log/Log.h"

#include "math/Math.h"

#include "loaders/GLTFLoader.h"

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

	/* Pre-Testing */

	Matrix4 translation = Matrix4().SetTranslation({0.0f, 0.0f, 1.0f});
	Matrix4 rot = Matrix4().SetRotation(Quaternion().SetAxisAngle({0.0f, 1.0f, 0.0f}, ToRadians(90.0f)));
	Matrix4 scale = Matrix4().SetScale({ 2.0f, 2.0f, 2.0f });

	Matrix4 mat = scale * rot * translation;

	Vector4 test = mat * Vector4(0, 0, 1, 1);

	Array<Model> models;
	LoadGLTF("assets\\gltf\\Helmet\\SciFiHelmet.gltf", models);

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