#pragma once

#include "platform/VPPlatform.h"
#include "graphics/VGFXContext.h"

#include "application/Input.h"
#include "application/WindowManager.h"

namespace Quartz
{
	struct GameInfo
	{
		StringW name;
		StringW version;
	};

	struct EngineInfo
	{
		GameInfo gameInfo;
		VPPlatform* pPlatform;
		VGFXContext* pGraphics;
		UInt32 showDebugConsole;
	};

	class QUARTZ_API Engine
	{
	private:
		static Engine* sInstance;

	private:
		GameInfo		mGameInfo;
		VPPlatform*		mpPlatform;
		VGFXContext*	mpGraphics;
		Input*			mpInput;
		WindowManager*	mpWindowManager;
		VPDebugConsole* mpDebugConsole;

	private:
		void Setup(const EngineInfo& info);
		void SetupDebugConsole(Bool8 showConsole);
		void SetupDebugLogging();

		void PrintSplash();

	public:
		static Engine& GetInstance();

		static Engine& CreateInstance(const EngineInfo& info);
		void Start();

		const GameInfo& GetGameInfo();

		VPPlatform& GetPlatform();
		VGFXContext& GetGraphics();
		Input& GetInput();
		WindowManager& GetWindowManager();
	};

}