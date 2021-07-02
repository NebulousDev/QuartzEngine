#include "GameModule.h"

#include "../Engine.h"
#include "../graphics2/renderers/SimpleRenderer.h"
#include "../log/Log.h"

namespace Quartz
{
	/*=====================================
		WINDOW/GRAPHICS PROPERITES
	=====================================*/

	#define WINDOW_WIDTH	1280//1920
	#define WINDOW_HEIGHT	720//1080

    Game::Game()
        : Module({ L"Game" }),
		pGameApp(nullptr),
		pGameWindow(nullptr)
    {
        // Nothing
    }

    Bool8 Game::PostInit()
    {
		ApplicationManager* pAppManager		= Engine::GetInstance()->GetApplicationManager();
		SceneManager*		pSceneManager	= Engine::GetInstance()->GetSceneManager();
		Graphics*			pGraphics		= Engine::GetInstance()->GetGraphics();

		/* Create Game Application */

		ApplicationInfo applicationInfo;
		applicationInfo.name	= L"Sandbox";
		applicationInfo.version = L"1.0.0";

		pGameApp = pAppManager->CreateManagedApplication(applicationInfo);

		Log::General("Game Application Created");

		/* Create Game Window */

		WindowInfo windowInfo;
		windowInfo.title		= L"Sandbox";
		windowInfo.position.x	= 80;
		windowInfo.position.y	= 80;
		windowInfo.size.x		= WINDOW_WIDTH;
		windowInfo.size.y		= WINDOW_HEIGHT;
		windowInfo.focused		= true;
		windowInfo.shown		= true;
		windowInfo.mode			= WINDOW_MODE_WINDOWED;

		pGameWindow = pGameApp->CreateWindow(windowInfo);

		/* Create Game Scene */

		pGameScene = pSceneManager->CreateScene("TestScene");

		/* Create Rendered Context */

		Surface*	pSurface		= pGraphics->CreateSurface(pGameWindow);
		Renderer*	pRenderer		= new SimpleRenderer();
		Context*	pGameViewport	= pGraphics->CreateContext(pSurface, pGameScene, pRenderer, MULTIBUFFER_TRIPPLE);

        return true;
    }

    void Game::Tick()
    {
		
    }

	void Game::Shutdown()
	{
		ApplicationManager* pAppManager		= Engine::GetInstance()->GetApplicationManager();
		SceneManager*		pSceneManager	= Engine::GetInstance()->GetSceneManager();
		Graphics*			pGraphics		= Engine::GetInstance()->GetGraphics();

		//pAppManager.DestroyApplication(pGameApp); // destroys all windows
	}
}

