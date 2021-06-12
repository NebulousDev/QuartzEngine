#include "Engine.h"

#include "log/Log.h"

namespace Quartz
{
	void Engine::Initialize(const EngineInfo& info)
	{
		mGameInfo = info.gameInfo;
		mpGraphics = info.pGraphicsModule;
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

		/* Setup Internal Modules */

		mpApplicationManager	= new ApplicationManager();
		mpSceneManager			= new SceneManager();

		// mpGraphics is set in constructor

		AddModule(mpApplicationManager);
		AddModule(mpSceneManager);
		AddModule(mpGraphics);

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
	}

	void Engine::Tick()
	{
		for (Module* pModule : mModules)
		{
			pModule->Tick();
		}
	}

	Engine* Engine::GetInstance()
	{
		static Engine* spInstance = new Engine();
		return spInstance;
	}
}

