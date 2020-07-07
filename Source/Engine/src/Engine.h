#pragma once

#include "Platform.h"
#include "util\Singleton.h"

#include "event\EventSystem.h"
#include "input\InputSystem.h"
#include "graphics\RenderSystem.h"
#include "log\DebugMessageSystem.h"
#include "input\DeviceConnectionSystem.h"

#include "graphics\GFXContext.h"
#include "PlatformWindow.h"

namespace Quartz
{
	class QUARTZ_API Engine : public Singleton<Engine>
	{
	public:
		friend class Singleton<Engine>;

	private:
		EventSystem				mEventSystem;
		InputSystem				mInputSystem;
		DeviceConnectionSystem	mDeviceConnectionSystem;
		RenderSystem			mRenderSystem;
		DebugMessageSystem		mDebugMessageSystem;

		Platform*				mpPlatform;
		GFXContext*				mpGFXContext;
		Window*					mpWindow;

		Bool8 mRunning = false;

		Time64 mDeltaTime;

		Engine() = default;

	private:
		void Run();

	public:
		void SetPlatform(Platform* pPlatform);
		void SetGraphicsContext(GFXContext* pGFXContext);
		void SetWindow(Window* pWindow);

		void Init();
		void Start();
		void Stop();

		Platform&					GetPlatform() { return *mpPlatform; }
		GFXContext&					GetGFXContext() { return *mpGFXContext; }
		Window&						GetWindow() { return *mpWindow; }

		EventSystem&				GetEventSystem() { return mEventSystem; }
		InputSystem&				GetInputSystem() { return mInputSystem; }
		DeviceConnectionSystem&		GetDeviceConnectionSystem() { return mDeviceConnectionSystem; }
		RenderSystem&				GetRenderSystem() { return mRenderSystem; }
		DebugMessageSystem&			GetDebugMessageSystem() { return mDebugMessageSystem; }

		Time64 GetDeltaTime() { return mDeltaTime; }

		Bool8 IsRunning() { return mRunning; }
	};
}