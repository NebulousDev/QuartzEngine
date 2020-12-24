#pragma once

#include "Common.h"
#include "util/String.h"
#include "util/Map.h"
#include "Window.h"

namespace Quartz
{
	typedef void(*WindowFocusLostCallback)(Window& window);
	typedef void(*WindowFocusGainedCallback)(Window& window);

	typedef void(*WindowResizedCallback)(Window& window, Point2u size);
	typedef void(*WindowMovedCallback)(Window& window, Point2i position);

	class QUARTZ_API WindowManager
	{
	protected:
		typedef Handle64 WindowHandle;

		Map<String, Window*> mWindows;

		WindowFocusLostCallback		mFocusLostCallback = nullptr;
		WindowFocusGainedCallback	mFocusGainedCallback = nullptr;
		WindowResizedCallback		mResizedCallback = nullptr;
		WindowMovedCallback			mMovedCallback = nullptr;

		Window*	mpActiveWindow;

	public:
		virtual Window& CreateWindow(StringW title, Int32 posX, Int32 posY, UInt32 clientWidth,
			UInt32 clientHeight, Bool8 fullscreen, Bool8 visible) = 0;

		virtual void Update() = 0;

		void SetFocusLostCallback(WindowFocusLostCallback callback);
		void SetFocusGainedCallback(WindowFocusGainedCallback callback);
		void SetResizedCallback(WindowResizedCallback callback);
		void SetMovedCallback(WindowMovedCallback callback);

		WindowFocusLostCallback GetFocusLostCallback();
		WindowFocusGainedCallback GetFocusGainedCallback();
		WindowResizedCallback GetResizedCallback();
		WindowMovedCallback GetMovedCallback();

		Window* GetActiveWindow();
	};
}