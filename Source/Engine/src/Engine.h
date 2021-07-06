#pragma once

#include "Common.h"

#include "Module.h"
#include "platform/Platform.h"
#include "application/ApplicationModule.h"
#include "graphics/GraphicsModule.h"
#include "graphics/SceneSystem.h"
#include "event/EventSystem.h"
#include "input/InputModule.h"

#include "util/Singleton.h"
#include "util/Array.h"
#include "util/String.h"


namespace Quartz
{
	/* Game Information */

	struct GameInfo
	{
		StringW		name;
		StringW		version;
	};

	/* Engine Information */

	struct EngineInfo
	{
		GameInfo	gameInfo;
		Graphics*	pGraphicsModule;
		Platform*	pPlatformModule;
		Float32		targetTPS;
	};

	/* Engine */

	class QUARTZ_API Engine
	{
	private:
		GameInfo			mGameInfo;
		Float32				mTargetTPS;

		Time*				mpTime;
		Float32				mCurrentTPS;
		Float32				mCurrentUPS;

		Graphics*			mpGraphics;
		Platform*			mpPlatform;
		ApplicationManager*	mpApplicationManager;
		EventSystem*		mpEventSystem;
		InputSystem*		mpInputSystem;
		SceneManager*		mpSceneManager;

		Array<Module*>		mModules;

		Bool8				mRunning;
		Bool8				mShutdownRequested;

	private:
		Engine() = default;

		void Update(Float32 delta);
		void Tick(UInt32 tick);

		void Shutdown();

		void RunEngineLoop();

	public:
		void Initialize(const EngineInfo& info);

		Bool8 AddModule(Module* pModule);

		Bool8 Start();
		
		void RequestShutdown();

		FORCE_INLINE const GameInfo&	GetGameInfo() { return mGameInfo; };
		FORCE_INLINE Bool8				IsRunning() { return mRunning; };

		FORCE_INLINE ApplicationManager*	GetApplicationManager() { return mpApplicationManager; }
		FORCE_INLINE EventSystem*			GetEventSystem() { return mpEventSystem; }
		FORCE_INLINE InputSystem*			GetInputSystem() { return mpInputSystem; }
		FORCE_INLINE SceneManager*			GetSceneManager() { return mpSceneManager; }
		FORCE_INLINE Graphics*				GetGraphics() { return mpGraphics; }
		FORCE_INLINE Platform*				GetPlatform() { return mpPlatform; }

		static Engine* GetInstance();
	};
}