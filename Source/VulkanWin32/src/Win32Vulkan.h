#pragma once

#include "VulkanContext.h"

#include "Win32Window.h"

namespace Quartz
{
	struct QUARTZ_API Win32VulkanSurface : public VulkanSurface
	{
		Win32Window* pWindow;
	};

	class QUARTZ_API Win32VulkanContext : public VPLVulkanContext
	{
	public:
		HGFXSurface CreateSurface(HVPWindow window, UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen) override;
		void DestroySurface(HGFXSurface surface) override;

		HGFXSwapchain CreateSwapchain(HGFXSurface surface, UInt32 bufferCount,
			UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen) override;
		void DestroySwapchain(HGFXSwapchain swapchain) override;
	};
}