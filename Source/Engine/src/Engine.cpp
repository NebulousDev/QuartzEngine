#include "Engine.h"

#include "log/Log.h"

namespace Quartz
{
	void Engine::Initialize(const EngineInfo& info)
	{
		/* Setup Info */

		mGameInfo	= info.gameInfo;
		mpGraphics	= info.pGraphicsModule;
		mpPlatform	= info.pPlatformModule;
		mpTime		= info.pPlatformModule->GetTime();
		mTargetTPS	= info.targetTPS;

		/* Setup Internal Modules */

		mpApplicationManager	= new ApplicationManager();
		mpEventSystem			= new EventSystem();
		mpInputSystem			= new InputSystem();
		mpSceneManager			= new SceneManager();

		// mpGraphics is set in constructor

		mSceneGraph.SetWorld(&mEntityWorld);

		AddModule(mpApplicationManager);
		AddModule(mpPlatform);
		AddModule(mpInputSystem);
		AddModule(mpSceneManager);
		AddModule(mpGraphics);
		AddModule(mpEventSystem);
	}

	Bool8 Engine::AddModule(Module* pModule)
	{
		if (mRunning)
		{
			Log::Error(L"Attempted to add new module '%s' while engine was running.", pModule->GetModuleName().Str());
			return false;
		}

		mModules.PushBack(pModule);
		return true;
	}

	Bool8 Engine::Start()
	{
		if (mRunning)
		{
			Log::Warning("Attempted to call Start() while engine was running.");
			return false;
		}

		/* Pre-Init */

		Log::General("Pre-initializing engine modules...");

		for (Module* pModule : mModules)
		{
			if (pModule->PreInit())
			{
				Log::Info(L"Module '%s' pre-initialized.", pModule->GetModuleName().Str());
			}
			else
			{
				Log::Critical(L"Module '%s' pre-initialization failed! Exiting.", pModule->GetModuleName().Str());
				return false;
			}
		}

		/* Init */

		Log::General("Initializing engine modules...");

		for (Module* pModule : mModules)
		{
			if (pModule->Init())
			{
				Log::Info(L"Module '%s' initialized.", pModule->GetModuleName().Str());
			}
			else
			{
				Log::Critical(L"Module '%s' initialization failed! Exiting.", pModule->GetModuleName().Str());
				return false;
			}
		}

		/* Post-Init */

		Log::General("Post-initializing engine modules...");

		for (Module* pModule : mModules)
		{
			if (pModule->PostInit())
			{
				Log::Info(L"Module '%s' post-initialized.", pModule->GetModuleName().Str());
			}
			else
			{
				Log::Critical(L"Module '%s' post-initialization failed! Exiting.", pModule->GetModuleName().Str());
				return false;
			}
		}

		/* Run */

		Log::General("Running engine...");

		mRunning = true;

		RunEngineLoop();
	}

	void Engine::RequestShutdown()
	{
		mShutdownRequested = true;
	}

	void Engine::RunEngineLoop()
	{
		Time64 currentTime			= 0;
		Time64 lastTime				= 0;
		Time64 deltaTime			= 0;
		Time64 accumulatedTime		= 0;
		Time64 accumulatedTickTime	= 0;
		UInt32 accumulatedUpdates	= 0;
		UInt32 accumulatedTicks		= 0;

		currentTime = mpTime->GetTimeNanoseconds();
		lastTime = currentTime;

		while (!mShutdownRequested)
		{
			accumulatedUpdates++;

			currentTime			= mpTime->GetTimeNanoseconds();
			deltaTime			= currentTime - lastTime;
			lastTime			= currentTime;

			accumulatedTime		+= deltaTime;
			accumulatedTickTime += deltaTime;

			mDelta = deltaTime;

			if (accumulatedTime >= 1.0)
			{
				mCurrentUPS = accumulatedUpdates;
				mCurrentTPS = accumulatedTicks;

				accumulatedUpdates = 0;
				accumulatedTicks = 0;
				accumulatedTime = 0;

				Log::Debug("UPS: %.2f, TPS: %.2f / %.2f", mCurrentUPS, mCurrentTPS, mTargetTPS);
			}

			if (accumulatedTickTime >= (1.0 / mTargetTPS))
			{
				accumulatedTicks++;

				Tick(accumulatedTicks);

				accumulatedTickTime = 0;
			}

			Update(deltaTime);
		}

		Shutdown();
	}

	void Engine::Update(Float32 delta)
	{
		for (Module* pModule : mModules)
		{
			pModule->PreUpdate(delta);
		}

		for (Module* pModule : mModules)
		{
			pModule->Update(delta);
		}

		for (Module* pModule : mModules)
		{
			pModule->PostUpdate(delta);
		}
	}

	void Engine::Tick(UInt32 tick)
	{
		for (Module* pModule : mModules)
		{
			pModule->PreTick(tick);
		}

		for (Module* pModule : mModules)
		{
			pModule->Tick(tick);
		}

		for (Module* pModule : mModules)
		{
			pModule->PostTick(tick);
		}
	}

	void Engine::Shutdown()
	{
		mRunning = false;
		
		for (Module* pModule : mModules)
		{
			pModule->PreShutdown();
		}

		for (Module* pModule : mModules)
		{
			pModule->Shutdown();
		}

		delete mpApplicationManager;
		delete mpEventSystem;
		delete mpSceneManager;
	}

	Engine* Engine::GetInstance()
	{
		static Engine* spInstance = new Engine();
		return spInstance;
	}
}

