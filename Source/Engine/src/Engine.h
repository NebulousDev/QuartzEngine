#pragma once

#include "Platform.h"
#include "util\Singleton.h"

#include "event\EventSystem.h"
#include "input\InputHandler.h"
#include "graphics\system\RenderSystem.h"
#include "log\DebugMessageSystem.h"
#include "input\ConnectionHandler.h"

#include "graphics\GFXContext.h"
#include "application/WindowManager.h"
#include "application/Window.h"

#include "entity/World.h"

namespace Quartz
{
	class QUARTZ_API Engine : public Singleton<Engine>
	{
	public:
		friend class Singleton<Engine>;

	private:
		EventSystem*			mpEventSystem;
		InputHandler*			mpInputHandler;
		ConnectionHandler*		mpConnectionHandler;
		RenderSystem*			mpRenderSystem;
		DebugMessageSystem*		mpDebugMessageSystem;

		EntityWorld				mWorld;

		Platform*				mpPlatform;
		GFXContext*				mpGFXContext;
		WindowManager*			mpWindowManager;
		Window*					mpWindow;

		Bool8 mRunning = false;

		Time64 mDeltaTime;

		Engine() = default;

	private:
		void Run();

	public:
		void SetPlatform(Platform* pPlatform);
		void SetGraphicsContext(GFXContext* pGFXContext);
		void SetWindowManager(WindowManager& manager);
		void SetWindow(Window* pWindow);

		void Init();
		void Start();
		void Stop();

		Platform&			GetPlatform() { return *mpPlatform; }
		GFXContext&			GetGFXContext() { return *mpGFXContext; }
		Window&				GetWindow() { return *mpWindow; }

		EventSystem&		GetEventSystem() { return *mpEventSystem; }
		InputHandler&		GetInputHandler() { return *mpInputHandler; }
		ConnectionHandler&	GetConnectionHandler() { return *mpConnectionHandler; }
		RenderSystem&		GetRenderSystem() { return *mpRenderSystem; }
		DebugMessageSystem&	GetDebugMessageSystem() { return *mpDebugMessageSystem; }
		WindowManager&		GetWindowManager() { return *mpWindowManager; }

		Time64 GetDeltaTime() { return mDeltaTime; }

		Bool8 IsRunning() { return mRunning; }
	};
}