#pragma once

#include "Common.h"

#include "Module.h"
#include "application/ApplicationModule.h"
#include "graphics/GraphicsModule.h"
#include "graphics/SceneSystem.h"

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
	};

	/* Engine */

	class QUARTZ_API Engine
	{
	private:
		GameInfo			mGameInfo;
		Bool8				mRunning;
		ApplicationManager*	mpApplicationManager;
		SceneManager*		mpSceneManager;
		Graphics*			mpGraphics;
		Array<Module*>		mModules;

	private:
		Engine() = default;

	public:
		void Initialize(const EngineInfo& info);

		Bool8 AddModule(Module* pModule);

		Bool8 Start();

		void Tick();
		
		FORCE_INLINE const GameInfo&	GetGameInfo() { return mGameInfo; };
		FORCE_INLINE Bool8				IsRunning() { return mRunning; };

		FORCE_INLINE ApplicationManager*	GetApplicationManager() { return mpApplicationManager; }
		FORCE_INLINE SceneManager*			GetSceneManager() { return mpSceneManager; }
		FORCE_INLINE Graphics*				GetGraphics() { return mpGraphics; }

		static Engine* GetInstance();
	};
}