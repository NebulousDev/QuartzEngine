#include "GameModule.h"

#include "../Engine.h"
#include "../graphics/renderers/SimpleRenderer.h"
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
		mpGameApp(nullptr),
		mpGameWindow(nullptr)
    {
        // Nothing
    }

	Bool8 Game::OnWindowClose(const WindowCloseEvent& event)
	{
		if (event.pWindow == mpGameWindow)
		{
			Engine::GetInstance()->RequestShutdown();
		}

		return true;
	}

	Bool8 Game::OnPlayerMove(const InputActionEvent& event)
	{
		if (event.name == "PlayerMove")
		{
			Log::Critical("PLAYER MOVE: { %.2f, %.2f, %.2f }, %.2f", 
				event.direction.x, event.direction.y, event.direction.z, event.value);
		}

		return true;
	}

	Bool8 Game::OnPlayerLook(const InputActionEvent& event)
	{
		if (event.name == "PlayerLook")
		{
			//Log::Critical("PLAYER LOOK: { %.2f, %.2f }, %.2f",
			//	event.direction.x, event.direction.y, event.value);
		}

		else if (event.name == "MouseWheel")
		{
			Log::Critical("MOUSE WHEEL: %.2f",
				event.direction.x, event.direction.y, event.value);
		}

		return true;
	}

	Bool8 Game::OnPeripheralConnection(const PeripheralEvent& event)
	{
		if (event.eventType == PERIPHERAL_CONNECTED)
		{
			Log::Info(L"Peripheral '%s' connected.", event.pPeripheral->deviceName.Str());
		}
		else if (event.eventType == PERIPHERAL_DISCONNECTED)
		{
			Log::Info(L"Peripheral '%s' disconnected.", event.pPeripheral->deviceName.Str());
		}

		return true;
	}

    Bool8 Game::PostInit()
    {
		ApplicationManager* pAppManager		= Engine::GetInstance()->GetApplicationManager();
		EventSystem*		pEventSystem	= Engine::GetInstance()->GetEventSystem();
		InputSystem*		pInputSystem	= Engine::GetInstance()->GetInputSystem();
		SceneManager*		pSceneManager	= Engine::GetInstance()->GetSceneManager();
		Graphics*			pGraphics		= Engine::GetInstance()->GetGraphics();

		/* Create Game Application */

		ApplicationInfo applicationInfo;
		applicationInfo.name	= L"Sandbox";
		applicationInfo.version = L"1.0.0";

		mpGameApp = pAppManager->CreateManagedApplication(applicationInfo);

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

		mpGameWindow = mpGameApp->CreateWindow(windowInfo);

		/* Create Game Scene */

		mpGameScene = pSceneManager->CreateScene("TestScene");

		/* Create Rendered Context */

		Surface*	mpSurface		= pGraphics->CreateSurface(mpGameWindow);
		Renderer*	mpRenderer		= new SimpleRenderer();
		Context*	mpGameViewport	= pGraphics->CreateContext(mpSurface, mpGameScene, mpRenderer, MULTIBUFFER_TRIPPLE);

		/* Bind Input Actions */

		pInputSystem->BindKeyboardInputAction("PlayerMove", INPUT_ANY_KEYBOARD, 17 /* W */, BUTTON_ACTION_ANY_DOWN, {  0.0f,  0.0f,  1.0f }, 1.0f);
		pInputSystem->BindKeyboardInputAction("PlayerMove", INPUT_ANY_KEYBOARD, 31 /* S */, BUTTON_ACTION_ANY_DOWN, {  0.0f,  0.0f, -1.0f }, 1.0f);
		pInputSystem->BindKeyboardInputAction("PlayerMove", INPUT_ANY_KEYBOARD, 30 /* A */, BUTTON_ACTION_ANY_DOWN, { -1.0f,  0.0f,  0.0f }, 1.0f);
		pInputSystem->BindKeyboardInputAction("PlayerMove", INPUT_ANY_KEYBOARD, 32 /* D */, BUTTON_ACTION_ANY_DOWN, {  1.0f,  0.0f,  0.0f }, 1.0f);
		
		pInputSystem->BindMouseMoveInputAction("PlayerLook", INPUT_ANY_MOUSE);

		/* Register Callbacks */

		pEventSystem->Subscribe<WindowCloseEvent>(this, &Game::OnWindowClose);
		pEventSystem->Subscribe<InputActionEvent>(this, &Game::OnPlayerMove);
		pEventSystem->Subscribe<InputActionEvent>(this, &Game::OnPlayerLook);
		pEventSystem->Subscribe<PeripheralEvent>(this, &Game::OnPeripheralConnection);

		pInputSystem->TriggerInputAction("PlayerMove", { }, 1.0f);
		pInputSystem->TriggerKeyboardInputAction((Keyboard*)5, 1, BUTTON_ACTION_DOWN);
		pInputSystem->TriggerMouseMoveInputAction((Mouse*)10, { 10.0f, 12.0f });

        return true;
    }

	void Game::Update(Float32 delta)
	{

	}

    void Game::Tick(UInt32 ticks)
    {
		
    }

	void Game::Shutdown()
	{
		ApplicationManager* pAppManager		= Engine::GetInstance()->GetApplicationManager();
		SceneManager*		pSceneManager	= Engine::GetInstance()->GetSceneManager();
		Graphics*			pGraphics		= Engine::GetInstance()->GetGraphics();

		// TODO: Somehow already deleted?
		//pGraphics->DestroyContext(mpGameViewport);
		//pGraphics->DestroySurface(mpSurface);

		//delete mpRenderer;

		//pAppManager.DestroyApplication(pGameApp); // destroys all windows
	}
}

