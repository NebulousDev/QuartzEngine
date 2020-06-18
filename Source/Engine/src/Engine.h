#pragma once

#include "Platform.h"
#include "util\Singleton.h"

#include "event\EventSystem.h"
#include "input\InputSystem.h"
#include "gfx\RenderSystem.h"
#include "log\DebugMessageSystem.h"
#include "input\DeviceConnectionSystem.h"

namespace Quartz
{
	class QUARTZ_API Engine : public Singleton<Engine>
	{
	public:
		friend class Singleton<Engine>;

	private:
		Platform*				mpPlatform;
		EventSystem				mEventSystem;
		InputSystem				mInputSystem;
		DeviceConnectionSystem	mDeviceConnectionSystem;
		RenderSystem			mRenderSystem;
		DebugMessageSystem		mDebugMessageSystem;

		Bool8 mRunning = false;

		Time64 mDeltaTime;

		Engine() {};

	private:
		void Run(Window* pWindow);

	public:
		void SetPlatform(Platform* pPlatform);

		void Init();
		void Start(Window* pWindow);
		void Stop();

		Platform&					GetPlatform() { return *mpPlatform; }

		EventSystem&				GetEventSystem() { return mEventSystem; }
		InputSystem&				GetInputSystem() { return mInputSystem; }
		DeviceConnectionSystem&		GetDeviceConnectionSystem() { return mDeviceConnectionSystem; }
		RenderSystem&				GetRenderSystem() { return mRenderSystem; }
		DebugMessageSystem&			GetDebugMessageSystem() { return mDebugMessageSystem; }

		Time64 GetDeltaTime() { return mDeltaTime; }

		Bool8 IsRunning() { return mRunning; }
	};
}