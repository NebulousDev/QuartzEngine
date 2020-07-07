#include "Engine.h"
#include "log\Log.h"

namespace Quartz
{
	void Engine::SetPlatform(Platform* pPlatform)
	{
		mpPlatform = pPlatform;
	}

	void Engine::SetGraphicsContext(GFXContext* pGFXContext)
	{
		mpGFXContext = pGFXContext;
	}

	void Engine::SetWindow(Window* pWindow)
	{
		mpWindow = pWindow;
	}

	void Engine::Run()
	{
		Time64 lastTime = 0.0;
		Time64 currentTime = 0.0;
		Time64 deltaTime = 0.0;
		Time64 elapsedTime = 0.0;

		UInt32 fps = 0;

		currentTime = mpPlatform->GetPlatformTime().GetTimeNanoseconds();

		while (true)
		{
			mpWindow->Update();

			mEventSystem.DispatchEvents();

			mDebugMessageSystem.Update();
			mDeviceConnectionSystem.Update();
			mInputSystem.Update();
			mRenderSystem.Update();

			lastTime = currentTime;
			currentTime = mpPlatform->GetPlatformTime().GetTimeNanoseconds();
			deltaTime = currentTime - lastTime;
			elapsedTime += deltaTime;

			if (elapsedTime >= 1.0)
			{
				mDebugMessageSystem.Tick();
				mDeviceConnectionSystem.Tick();
				mInputSystem.Tick();
				mRenderSystem.Tick();
				
				Log.Debug("MS: %.4llfms, FPS: %d", deltaTime * 1000.0, fps);
				elapsedTime = 0.0;
				fps = 0;
			}

			++fps;
		}
	}

	void Engine::Init()
	{
		mEventSystem.InitializeEventSystem();
		mDebugMessageSystem.Initialize();

		Log.Print("-------------------------------------------------------\n");
		Log.Print("|                    Nebulous Games                   |\n");
		Log.Print("|                 QUARTZ ENGINE v0.2.0                |\n");
		Log.Print("-------------------------------------------------------\n");

		Log.Info("Engine is starting...");

		mDeviceConnectionSystem.Initialize();
		mInputSystem.Initialize();
		mRenderSystem.Initialize();
	}

	void Engine::Start()
	{
		if (mRunning)
		{
			Log.Warning("Engine.Start() was called while the engine is running.");
			return;
		}

		mRunning = true;

		Run();
	}

	void Engine::Stop()
	{
		mInputSystem.Destroy();
		mDeviceConnectionSystem.Destroy();
		mRenderSystem.Destroy();
		mDebugMessageSystem.Destroy();

		mRunning = false;
	}
}

