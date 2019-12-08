#include "Engine.h"
#include "io\Log.h"

namespace Quartz
{
	void Engine::Start()
	{
		if (!mInitialized)
		{
			mInitialized = true;
		}

		else
		{
			Log.Warning("Engine.Start() was called while the engine is running.");
		}
	}

	void Engine::Stop()
	{
		mRunning = false;
	}

	void Engine::Update()
	{

	}
}

