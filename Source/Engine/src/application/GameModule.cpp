#include "GameModule.h"

#include "../Engine.h"
#include "../graphics/renderers/SimpleRenderer.h"
#include "../log/Log.h"

#include "../entity/basic/Transform.h"
#include "../graphics/component/Camera.h"
#include "../graphics/component/Mesh.h"
#include "../graphics/component/Material.h"

namespace Quartz
{
	/*=====================================
		WINDOW/GRAPHICS PROPERITES
	=====================================*/

	#define WINDOW_WIDTH	1920
	#define WINDOW_HEIGHT	1080

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
		
		mCamera = mpGameScene->GetWorld().CreateEntity
		(
			TransformComponent({ 0.0f, 0.0f, 0.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 1.0f, 1.0f, 1.0f }),
			CameraComponent{ Matrix4().SetPerspective(ToRadians(80.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f) }
		);

		mpGameScene->SetCamera(mCamera);

		mEntity1 = mpGameScene->GetWorld().CreateEntity
		(
			TransformComponent({ 0.0f, 0.0f, 0.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 1.0f, 1.0f, 1.0f }),
			MeshComponent("assets/models/testScene.obj"),
			MaterialComponent
			(
				"assets/textures/wood_diffuse.png",
				"assets/textures/wood_normal.png",
				"assets/textures/shiney_roughness.png",
				"assets/textures/wood_metalic.png",
				"assets/textures/wood_ao.png"
			)
			
		);

		mEntity2 = mpGameScene->GetWorld().CreateEntity
		(
			TransformComponent({ 0.0f, 2.0f, 0.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 1.0f, 1.0f, 1.0f }),
			MeshComponent("assets/models/gun.obj"),
			MaterialComponent
			(
				"assets/textures/gun_diffuse.tga",
				"assets/textures/gun_normal.tga",
				"assets/textures/gun_roughness.tga",
				"assets/textures/gun_metallicness.tga",
				"assets/textures/gun_ambient.tga"
			)
		);

		mpGameScene->GetWorld().CreateEntity
		(
			TransformComponent({ 0.0f, 0.0f, -12.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 1.0f, 1.0f, 1.0f }),
			MeshComponent("assets/models/cube.obj"),
			MaterialComponent
			(
				"assets/textures/tile.png",
				"assets/textures/tile_normal.png",
				"assets/textures/shiney_roughness.png",
				"assets/textures/wood_metalic.png",
				"assets/textures/wood_metalic.png"
			)
		);

		/* Create Rendered Context */

		Surface*	mpSurface		= pGraphics->CreateSurface(mpGameWindow);
		Renderer*	mpRenderer		= new SimpleRenderer();
		Context*	mpGameViewport	= pGraphics->CreateContext(mpSurface, mpGameScene, mpRenderer, MULTIBUFFER_TRIPPLE);

		/* Bind Input Actions */

		pInputSystem->BindKeyboardInputAction("PlayerMoveForward",	INPUT_ANY_KEYBOARD, 17 /* W */, INPUT_ACTION_ANY, {  0.0f,  0.0f,  1.0f }, 1.0f);
		pInputSystem->BindKeyboardInputAction("PlayerMoveBackward", INPUT_ANY_KEYBOARD, 31 /* S */, INPUT_ACTION_ANY, {  0.0f,  0.0f, -1.0f }, 1.0f);
		pInputSystem->BindKeyboardInputAction("PlayerMoveLeft",		INPUT_ANY_KEYBOARD, 30 /* A */, INPUT_ACTION_ANY, {  1.0f,  0.0f,  0.0f }, 1.0f);
		pInputSystem->BindKeyboardInputAction("PlayerMoveRight",	INPUT_ANY_KEYBOARD, 32 /* D */, INPUT_ACTION_ANY, { -1.0f,  0.0f,  0.0f }, 1.0f);
		
		pInputSystem->BindMouseMoveInputAction("PlayerLook", INPUT_ANY_MOUSE);

		/* Register Callbacks */

		pEventSystem->Subscribe<WindowCloseEvent>(this, &Game::OnWindowClose);
		pEventSystem->Subscribe<PeripheralEvent>(this, &Game::OnPeripheralConnection);

        return true;
    }

	void Game::Update(Float32 delta)
	{
		constexpr Float32 moveSpeed = 12.0f;
		constexpr Float32 lookSpeed = 8.0f;

		InputSystem* pInputSystem = Engine::GetInstance()->GetInputSystem();

		ActionState playerMoveForward	= pInputSystem->GetInputAction("PlayerMoveForward");
		ActionState playerMoveBackward	= pInputSystem->GetInputAction("PlayerMoveBackward");
		ActionState playerMoveLeft		= pInputSystem->GetInputAction("PlayerMoveLeft");
		ActionState playerMoveRight		= pInputSystem->GetInputAction("PlayerMoveRight");

		ActionState playerLook = pInputSystem->GetInputAction("PlayerLook");
		
		TransformComponent& transform = mpGameScene->GetWorld().GetComponent<TransformComponent>(mCamera);
		Vector3 direction = { 0.0f, 0.0f, 0.0f };

		if(playerMoveForward.action & INPUT_ACTION_ANY_DOWN)
		{
			direction += playerMoveForward.axis * playerMoveForward.value;
		}

		if (playerMoveBackward.action & INPUT_ACTION_ANY_DOWN)
		{
			direction += playerMoveBackward.axis * playerMoveBackward.value;
		}

		if (playerMoveLeft.action & INPUT_ACTION_ANY_DOWN)
		{
			direction += playerMoveLeft.axis * playerMoveLeft.value;
		}

		if (playerMoveRight.action & INPUT_ACTION_ANY_DOWN)
		{
			direction += playerMoveRight.axis * playerMoveRight.value;
		}
		
		direction.Normalize();
		direction = transform.rotation * direction;

		transform.position += (direction * moveSpeed * delta);

		if (playerLook.action & INPUT_ACTION_ACTIVE)
		{
			Vector3 left = transform.rotation * Vector3( 1.0f, 0.0f, 0.0f );
			Vector3 up = { 0.0f, 1.0f, 0.0f };

			transform.rotation *= Quaternion().SetAxisAngle(up, playerLook.axis.x * playerLook.value * lookSpeed * delta)
							    * Quaternion().SetAxisAngle(left, playerLook.axis.y * playerLook.value * lookSpeed * delta);
		}

		// Rotate Entity

		TransformComponent& e2Transform = mpGameScene->GetWorld().GetComponent<TransformComponent>(mEntity2);
		e2Transform.rotation *= Quaternion().SetAxisAngle({ 0.0f, 1.0f, 0.0f }, -1.0f * delta);
		
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

