#pragma once

#include "Window.h"
#include "util/Map.h"

namespace Quartz
{
	using WindowMovedCallbackFunc = void (*)(Window& window, UInt32 x, UInt32 y);
	using WindowResizedCallbackFunc = void (*)(Window& window, UInt32 width, UInt32 height);
	using WindowFocusCallbackFunc = void (*)(Window& window, Bool8 focused);
	using WindowMouseEnteredCallbackFunc = void (*)(Window& window, Bool8 entered);

	class QUARTZ_API WindowManager
	{
	private:
		struct WindowMovedCallback
		{
			WindowMovedCallbackFunc callback;
		};

		struct WindowResizedCallback
		{
			WindowResizedCallbackFunc callback;
		};

		struct WindowFocusCallback
		{
			WindowFocusCallbackFunc callback;
		};

		struct WindowMouseEnteredCallback
		{
			WindowMouseEnteredCallbackFunc callback;
		};

		Map<HVPWindow, Window*> mWindows;
		Map<HVPWindow, Array<WindowMovedCallback>> mWindowMovedCallbacks;
		Map<HVPWindow, Array<WindowResizedCallback>> mWindowResizedCallbacks;
		Map<HVPWindow, Array<WindowFocusCallback>> mWindowFocusCallbacks;
		Map<HVPWindow, Array<WindowMouseEnteredCallback>> mWindowMouseEnteredCallbacks;

	private:
		friend void BindingWindowMovedCallback(HVPWindow window, UInt32 x, UInt32 y);
		friend void BindingWindowResizedCallback(HVPWindow window, UInt32 width, UInt32 height);
		friend void BindingWindowFocusCallback(HVPWindow window, Bool8 focused);
		friend void BindingWindowMouseEnteredCallback(HVPWindow window, Bool8 entered);

		void SendWindowMovedCallback(HVPWindow window, UInt32 x, UInt32 y);
		void SendWindowResizedCallback(HVPWindow window, UInt32 width, UInt32 height);
		void SendWindowFocusCallback(HVPWindow window, Bool8 focused);
		void SendWindowMouseEnteredCallback(HVPWindow window, Bool8 entered);

	public:
		void PreInitialize();
		void Initialize();

		GraphicsWindow* CreateGraphicsWindow(UInt32 posX, UInt32 posY, UInt32 width, 
			UInt32 height, const StringW& title, UInt32 backbufferCount);
		void DestroyGraphicsWindow(GraphicsWindow* pWindow);

		void RegisterWindowMovedCallback(Window& window, WindowMovedCallbackFunc callback);
		void RegisterWindowResizedCallback(Window& window, WindowResizedCallbackFunc callback);
		void RegisterWindowFocusCallback(Window& window, WindowFocusCallbackFunc callback);
		void RegisterWindowMouseEnteredCallback(Window& window, WindowMouseEnteredCallbackFunc callback);
	};
}