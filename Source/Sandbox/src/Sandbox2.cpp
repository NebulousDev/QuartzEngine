#include "Common.h"

#include "Engine.h"

namespace Quartz
{

	
}

int main(int argc, char* argv[])
{
	using namespace Quartz;

	/*=====================================
		ENGINE INITIALIZATION
	=====================================*/

	EngineInfo engineInfo;
	engineInfo.gameInfo.name		= L"Sandbox";
	engineInfo.gameInfo.version		= L"1.0.0";
	engineInfo.pPlatform			= &win32Platform;
	engineInfo.pGraphics			= &win32VulkanContext;
	engineInfo.showDebugConsole		= true;

	Engine& engine = Engine::CreateInstance(engineInfo);
	engine.Start();
}