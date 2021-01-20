#include "WindowManager.h"

#include "../Engine.h"

namespace Quartz
{
	void BindingWindowMovedCallback(HVPWindow window, UInt32 x, UInt32 y)
	{
		WindowManager& manager = Engine::GetInstance().GetWindowManager();
		manager.SendWindowMovedCallback(window, x, y);
	}

	void BindingWindowResizedCallback(HVPWindow window, UInt32 width, UInt32 height)
	{
		WindowManager& manager = Engine::GetInstance().GetWindowManager();
		manager.SendWindowResizedCallback(window, width, height);
	}

	void BindingWindowFocusCallback(HVPWindow window, Bool8 focus)
	{
		WindowManager& manager = Engine::GetInstance().GetWindowManager();
		manager.SendWindowFocusCallback(window, focus);
	}

	void BindingWindowMouseEnteredCallback(HVPWindow window, Bool8 entered)
	{
		WindowManager& manager = Engine::GetInstance().GetWindowManager();
		manager.SendWindowMouseEnteredCallback(window, entered);
	}

	void WindowManager::SendWindowMovedCallback(HVPWindow window, UInt32 x, UInt32 y)
	{
		Window** ppWindow = mWindows.Get(window);

		if (!ppWindow)
		{
			// No registered windows;
			return;
		}

		auto* pCallbacks = mWindowMovedCallbacks.Get(window);

		if (pCallbacks)
		{
			for (WindowMovedCallback& callback : *pCallbacks)
			{
				callback.callback(**ppWindow, x, y);
			}
		}
	}

	void WindowManager::SendWindowResizedCallback(HVPWindow window, UInt32 width, UInt32 height)
	{
		Window** ppWindow = mWindows.Get(window);

		if (!ppWindow)
		{
			// No registered windows;
			return;
		}

		auto* pCallbacks = mWindowResizedCallbacks.Get(window);

		if (pCallbacks)
		{
			for (WindowResizedCallback& callback : *pCallbacks)
			{
				callback.callback(**ppWindow, width, height);
			}
		}
	}

	void WindowManager::SendWindowFocusCallback(HVPWindow window, Bool8 focused)
	{
		Window** ppWindow = mWindows.Get(window);

		if (!ppWindow)
		{
			// No registered windows;
			return;
		}

		auto* pCallbacks = mWindowFocusCallbacks.Get(window);

		if (pCallbacks)
		{
			for (WindowFocusCallback& callback : *pCallbacks)
			{
				callback.callback(**ppWindow, focused);
			}
		}
	}

	void WindowManager::SendWindowMouseEnteredCallback(HVPWindow window, Bool8 entered)
	{
		Window** ppWindow = mWindows.Get(window);

		if (!ppWindow)
		{
			// No registered windows;
			return;
		}

		auto* pCallbacks = mWindowMouseEnteredCallbacks.Get(window);

		if (pCallbacks)
		{
			for (WindowMouseEnteredCallback& callback : *pCallbacks)
			{
				callback.callback(**ppWindow, entered);
			}
		}
	}

	void WindowManager::PreInitialize()
	{

	}

	void WindowManager::Initialize()
	{
		VPPlatform& platform = Engine::GetInstance().GetPlatform();
		platform.SetWindowMovedCallback(BindingWindowMovedCallback);
		platform.SetWindowResizedCallback(BindingWindowResizedCallback);
		platform.SetWindowFocusCallback(BindingWindowFocusCallback);
		platform.SetWindowMouseEnteredCallback(BindingWindowMouseEnteredCallback);
	}

	GraphicsWindow* WindowManager::CreateGraphicsWindow(UInt32 posX, UInt32 posY, UInt32 width, UInt32 height, const StringW& title, UInt32 backbufferCount)
	{
		Engine& engine = Engine::GetInstance();

		HVPWindow hWindow = engine.GetPlatform().CreateWindow(posX, posY, width, height, title);
		HGFXSurface hSurface = engine.GetGraphics().CreateSurface(hWindow, width, height, false, false);
		HGFXSwapchain hSwapchain = engine.GetGraphics().CreateSwapchain(hSurface, backbufferCount, width, height, false, false);
	
		GraphicsWindow* pGraphicsWindow = new GraphicsWindow(hWindow, hSurface, hSwapchain, posX, posY, width, height, title);

		mWindows[hWindow] = pGraphicsWindow;

		return pGraphicsWindow;
	}

	void WindowManager::DestroyGraphicsWindow(GraphicsWindow* pWindow)
	{
		Engine& engine = Engine::GetInstance();

		engine.GetPlatform().DestroyWindow(pWindow->GetNativeWindow());
		engine.GetGraphics().DestroySwapchain(pWindow->GetSwapchain());
		engine.GetGraphics().DestroySurface(pWindow->GetSurface());

		// @TODO: Remove window from mWindows map

		delete pWindow;
	}

	void WindowManager::RegisterWindowFocusCallback(Window& window, WindowFocusCallbackFunc callback)
	{
		mWindowFocusCallbacks[window.GetNativeWindow()]
			.PushBack(WindowFocusCallback{ callback });
	}

	void WindowManager::RegisterWindowMouseEnteredCallback(Window& window, WindowMouseEnteredCallbackFunc callback)
	{
		mWindowMouseEnteredCallbacks[window.GetNativeWindow()]
			.PushBack(WindowMouseEnteredCallback{ callback });
	}
}

