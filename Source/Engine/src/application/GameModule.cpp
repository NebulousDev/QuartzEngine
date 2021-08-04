#include "GameModule.h"

#include "../Engine.h"
#include "../graphics/renderers/SimpleRenderer.h"
#include "../log/Log.h"

#include "../physics/component/Transform.h"
#include "../graphics/component/Camera.h"
#include "../graphics/component/Mesh.h"
#include "../graphics/component/Material.h"
#include "../graphics/component/Light.h"

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

		EntityWorld*		pEntityWorld	= Engine::GetInstance()->GetEntityWorld();
		SceneGraph*			pSceneGraph		= Engine::GetInstance()->GetSceneGraph();

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

		Entity sceneRoot = pEntityWorld->CreateEntity();
		pSceneGraph->ParentEntityToRoot(sceneRoot);

		mCamera = pEntityWorld->CreateEntity
		(
			TransformComponent({ 0.0f, 0.0f, 0.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 1.0f, 1.0f, 1.0f }),
			CameraComponent{ Matrix4().SetPerspective(ToRadians(80.0f), 1920.0f / 1080.0f, 0.01f, 1000.0f) }
		);

		pSceneGraph->ParentEntity(mCamera, sceneRoot);

		mEntity1 = pEntityWorld->CreateEntity
		(
			TransformComponent({ 0.0f, 0.0f, 0.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 1.0f, 1.0f, 1.0f }),
			MeshComponent("assets/models/testscene.obj"),
			MaterialComponent
			(
				"assets/textures/wood_diffuse.png",
				"assets/textures/wood_normal.png",
				"assets/textures/shiney_roughness.png",
				"assets/textures/wood_metalic.png",
				"assets/textures/wood_ao.png"

				/*
				"assets/textures/gold_diffuse.jpg",
				"assets/textures/gold_normal.jpg",
				"assets/textures/gold_roughness.jpg",
				"assets/textures/gold_metallic.jpg",
				"assets/textures/gold_roughness.jpg"
				*/
			)
			
		);

		pSceneGraph->ParentEntity(mEntity1, sceneRoot);

		mEntity2 = pEntityWorld->CreateEntity
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

		pSceneGraph->ParentEntity(mEntity2, sceneRoot);
		pSceneGraph->ParentEntity(mCamera, mEntity2);

		Entity e1 = pEntityWorld->CreateEntity
		(
			TransformComponent({ 0.0f, 0.0f, 0.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 0.5f, 0.5f, 0.5f }),
			MeshComponent("assets/models/sponza2.obj"),
			MaterialComponent
			(
				"assets/textures/tile.png",
				"assets/textures/tile_normal.png",
				"assets/textures/dull_roughness.png",
				"assets/textures/wood_metalic.png",
				"assets/textures/wood_metalic.png"
			)
		);

		pSceneGraph->ParentEntity(e1, sceneRoot);

		mLight = pEntityWorld->CreateEntity
		(
			TransformComponent({ 0.0f, 0.0f, 0.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 0.5f, 0.5f, 0.5f }),
			LightComponent({ 0, 0, 0 })
		);

		pSceneGraph->ParentEntity(mLight, mCamera);

		Entity light0 = pEntityWorld->CreateEntity
		(
			TransformComponent({ -5.0f, 5.0f, 0.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 1.0f, 1.0f, 1.0f }),
			LightComponent({ 1000.0f, 0.0f, 0.0f })
		);

		pSceneGraph->ParentEntity(light0, sceneRoot);

		Entity light1 = pEntityWorld->CreateEntity
		(
			TransformComponent({ 0.0f, 5.0f, 0.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 1.0f, 1.0f, 1.0f }),
			LightComponent({ 0.0f, 1000.0f, 0.0f })
		);

		pSceneGraph->ParentEntity(light1, sceneRoot);

		Entity light2 = pEntityWorld->CreateEntity
		(
			TransformComponent({ 5.0f, 5.0f, 0.0f }, Quaternion().SetAxisAngle({}, 0.0f), { 1.0f, 1.0f, 1.0f }),
			LightComponent({ 0.0f, 0.0f, 1000.0f })
		);

		pSceneGraph->ParentEntity(light2, sceneRoot);

		mpGameScene = pSceneManager->CreateScene("TestScene", Engine::GetInstance()->GetSceneGraph()->CreateRootView(), mCamera);

		/* Create Rendered Context */

		Surface*	mpSurface		= pGraphics->CreateSurface(mpGameWindow);
		Renderer*	mpRenderer		= new SimpleRenderer();
		Context*	mpGameViewport	= pGraphics->CreateContext(mpSurface, mpGameScene, mpRenderer, MULTIBUFFER_TRIPPLE);

		/* Bind Input Actions */

		pInputSystem->BindKeyboardInputAction("PlayerMoveForward",	INPUT_ANY_KEYBOARD, 17 /* W */, INPUT_ACTION_ANY, {  0.0f,  0.0f, -1.0f }, 1.0f);
		pInputSystem->BindKeyboardInputAction("PlayerMoveBackward", INPUT_ANY_KEYBOARD, 31 /* S */, INPUT_ACTION_ANY, {  0.0f,  0.0f,  1.0f }, 1.0f);
		pInputSystem->BindKeyboardInputAction("PlayerMoveLeft",		INPUT_ANY_KEYBOARD, 30 /* A */, INPUT_ACTION_ANY, { -1.0f,  0.0f,  0.0f }, 1.0f);
		pInputSystem->BindKeyboardInputAction("PlayerMoveRight",	INPUT_ANY_KEYBOARD, 32 /* D */, INPUT_ACTION_ANY, {  1.0f,  0.0f,  0.0f }, 1.0f);
		
		pInputSystem->BindMouseMoveInputAction("PlayerLook", INPUT_ANY_MOUSE);

		pInputSystem->BindKeyboardInputAction("ToggleCameraLight", INPUT_ANY_KEYBOARD, 18 /* E */, INPUT_ACTION_ANY, { 0.0f,  0.0f,  0.0f }, 1.0f);

		/* Register Callbacks */

		pEventSystem->Subscribe<WindowCloseEvent>(this, &Game::OnWindowClose);
		pEventSystem->Subscribe<PeripheralEvent>(this, &Game::OnPeripheralConnection);

        return true;
    }

	void Game::Update(Float32 delta)
	{
		Engine* pEngine = Engine::GetInstance();

		constexpr Float32 moveSpeed = 8.0f;
		constexpr Float32 lookSpeed = 6.0f;

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
			Vector3 left = transform.GetLeft();
			Vector3 up = { 0.0f, 1.0f, 0.0f };

			transform.rotation *= Quaternion().SetAxisAngle(up,   playerLook.axis.x * playerLook.value * lookSpeed * delta)
							    * Quaternion().SetAxisAngle(left, playerLook.axis.y * playerLook.value * lookSpeed * delta);

		}

		Engine::GetInstance()->GetSceneGraph()->Update(mCamera);

		// Rotate Entity

		TransformComponent& e2Transform = mpGameScene->GetWorld().GetComponent<TransformComponent>(mEntity2);
		e2Transform.rotation *= Quaternion().SetAxisAngle({ 0.0f, 1.0f, 0.0f }, -1.0f * delta);

		Engine::GetInstance()->GetSceneGraph()->Update(mEntity2);

		ActionState toggleLight = pInputSystem->GetInputAction("ToggleCameraLight");

		if (toggleLight.action & INPUT_ACTION_PRESSED)
		{
			LightComponent& light = mpGameScene->GetWorld().GetComponent<LightComponent>(mLight);

			if (light.radiance.x == 0)
			{
				light.radiance = { 500.0f, 500.0f, 500.0f };
			}
			else
			{
				light.radiance = { 0, 0, 0 };
			}
		}
	}

    void Game::Tick(UInt32 ticks)
    {
		Engine* pEngine = Engine::GetInstance();
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

