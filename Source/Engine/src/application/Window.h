#pragma once

#include "../platform/VPPlatform.h"
#include "../graphics/VGFXContext.h"

namespace Quartz
{
	class QUARTZ_API Window
	{
	public:
		friend class WindowManager;

	protected:
		HVPWindow mWindow;

	public:
		Window(HVPWindow window, UInt32 x, UInt32 y, UInt32 width, UInt32 height, const StringW& title);

		void Show();
		void Hide();
		void Focus();

		HVPWindow GetNativeWindow() const;

		Point2i GetPosition() const;
		Point2i GetSize() const;
		Bounds2i GetBounds() const;
		UInt32 GetWidth() const;
		UInt32 GetHeight() const;
		Bool8 IsFocused() const;
		Bool8 IsMouseInside() const;
	};

	class QUARTZ_API GraphicsWindow : public Window
	{
	public:
		friend class WindowManager;

	protected:
		HGFXSurface mSurface;
		HGFXSwapchain mSwapchain;

	public:
		GraphicsWindow(HVPWindow window, HGFXSurface surface, HGFXSwapchain swapchain,
			UInt32 x, UInt32 y, UInt32 width, UInt32 height, const StringW& title);

		HGFXSurface GetSurface() const;
		HGFXSwapchain GetSwapchain() const;
	};
}