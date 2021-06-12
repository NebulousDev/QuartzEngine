#include "Window.h"

#include "../Engine.h"
#include "../platform/VPPlatform.h"

namespace Quartz
{
	Window::Window(HVPWindow window, UInt32 x, UInt32 y, UInt32 width, UInt32 height, const StringW& title) :
		mWindow(window)
	{
		// Nothing
	}

	void Window::Show()
	{
		Platform& platform = Engine::GetInstance().GetPlatform();
		platform.ShowWindow(mWindow, true);
	}

	void Window::Hide()
	{
		Platform& platform = Engine::GetInstance().GetPlatform();
		platform.ShowWindow(mWindow, false);
	}

	void Window::Focus()
	{
		Platform& platform = Engine::GetInstance().GetPlatform();
		platform.FocusWindow(mWindow);
	}

	HVPWindow Window::GetNativeWindow() const
	{
		return mWindow;
	}

	Point2i Window::GetPosition() const
	{
		Platform& platform = Engine::GetInstance().GetPlatform();
		return platform.GetWindowClientBounds(mWindow).start;
	}

	Point2i Window::GetSize() const
	{
		Platform& platform = Engine::GetInstance().GetPlatform();
		return platform.GetWindowClientSize(mWindow);
	}

	Bounds2i Window::GetBounds() const
	{
		Platform& platform = Engine::GetInstance().GetPlatform();
		return platform.GetWindowClientBounds(mWindow);
	}

	UInt32 Window::GetWidth() const
	{
		return GetBounds().Width();
	}

	UInt32 Window::GetHeight() const
	{
		return GetBounds().Height();
	}

	Bool8 Window::IsFocused() const
	{
		Platform& platform = Engine::GetInstance().GetPlatform();
		return platform.IsWindowFocus(mWindow);
	}

	Bool8 Window::IsMouseInside() const
	{
		Platform& platform = Engine::GetInstance().GetPlatform();
		return platform.IsMouseInsideWindowClient(mWindow);
	}

	GraphicsWindow::GraphicsWindow(HVPWindow window, HGFXSurface surface, HGFXSwapchain swapchain, 
		UInt32 x, UInt32 y, UInt32 width, UInt32 height, const StringW& title) :
		Window(window, x, y, width, height, title),
		mSurface(surface),
		mSwapchain(swapchain)
	{
		// Nothing
	}

	HGFXSurface GraphicsWindow::GetSurface() const
	{
		return mSurface;
	}

	HGFXSwapchain GraphicsWindow::GetSwapchain() const
	{
		return mSwapchain;
	}
}

