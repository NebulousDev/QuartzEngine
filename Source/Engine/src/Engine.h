#pragma once

#include "platform/VPPlatform.h"
#include "graphics/VGFXContext.h"

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
		static GameInfo mGameInfo;
		static VPPlatform* mpPlatform;
		static VGFXContext* mpGraphics;

		static VPDebugConsole* mpDebugConsole;

	private:
		static void SetupDebugConsole(Bool8 showConsole);
		static void SetupDebugLogging();
		static void PrintSplash();

	public:
		static void Setup(const EngineInfo& info);
		static void Start();

		// @Todo: find out why this cant be defined here?
		static const GameInfo& GetGameInfo();

		static VPPlatform& GetPlatformContext();
		static VGFXContext& GetGraphicsContext();
	};

}