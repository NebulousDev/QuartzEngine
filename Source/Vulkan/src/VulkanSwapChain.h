#pragma once

#include "Common.h"
#include "VulkanDevice.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanSwapChain
	{
	private:
		VkSwapchainKHR	mSwapChain;
		Array<VkImage>	mImages;

		UInt32 mWidth;
		UInt32 mHeight;

		struct
		{
			Bool8 mIsFullscreen		: 1;
			Bool8 mIsVSynced		: 1;
			Bool8 mIsValidSwapChain : 1;
		};

	public:
		VulkanSwapChain(VkInstance instance, VulkanDevice& device, VkSurfaceKHR surface,
			UInt32 bufferCount, UInt32 width, UInt32 height, Bool8 fullscreen, UInt32 vSync);

		const VkSwapchainKHR& GetSwapChainHandle() const { return mSwapChain; }

		const Array<VkImage>& GetImages() const { return mImages; }

		Bool8 IsFullscreen() const { return mIsFullscreen; }
		Bool8 IsVSynced() const { return mIsVSynced; }

		Bool8 IsValidSwapChain() const { return mIsValidSwapChain; }
	};
}