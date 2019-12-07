#include "Engine.h"
#include "io\Log.h"

namespace Quartz
{
	void Engine::Run()
	{
		if (!mRunning)
		{
			mRunning = true;

			while (mRunning)
			{
				Update();
			}
		}

		else
		{
			Log.Warning("Engine.Run() was called while the engine is running.");
		}
	}

	void Engine::Update()
	{

	}

	void Engine::Start()
	{
		if (!mInitialized)
		{
			mInitialized = true;
			Run();
		}

		else
		{
			Log.Warning("Engine.Start() was called while the engine is running.");
		}
	}
}

