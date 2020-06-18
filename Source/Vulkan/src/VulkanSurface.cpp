#include "VulkanSurface.h"
#include "VulkanImageView.h"

#include "win32/Win32Vulkan.h"

#include "log/log.h"

namespace Quartz
{
	Bool8 EnumerateSurfaceFormats(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, Array<VkSurfaceFormatKHR>& surfaceFormats)
	{
		VkResult result;
		UInt32 formatCount = 0;

		result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
		surfaceFormats.Resize(formatCount);
		result = vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.Data());

		return result == VK_SUCCESS;
	}

	Bool8 EnumeratePresentModes(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, Array<VkPresentModeKHR>& presentModes)
	{
		VkResult result;
		UInt32 modeCount = 0;

		result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &modeCount, nullptr);
		presentModes.Resize(modeCount);
		result = vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &modeCount, presentModes.Data());

		return result == VK_SUCCESS;
	}

	Bool8 RetrieveSwapChainImages(VkDevice device, VkSwapchainKHR swapChain, Array<VkImage>& images)
	{
		VkResult result;
		UInt32 swapChainImageCount = 0;

		result = vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, nullptr);
		images.Resize(swapChainImageCount);
		result = vkGetSwapchainImagesKHR(device, swapChain, &swapChainImageCount, images.Data());

		return result == VK_SUCCESS;
	}

	Bool8 PickSurfaceFormat(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceFormatKHR* pSelectedFormat)
	{
		Array<VkSurfaceFormatKHR> availableFormats;

		if (!EnumerateSurfaceFormats(physicalDevice, surface, availableFormats))
		{
			Log.Error("Failed to pick surface format: Unable to enumerate surface formats!");
			return false;
		}

		for (const VkSurfaceFormatKHR& format : availableFormats)
		{
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				*pSelectedFormat = format;
				return true;
			}
		}

		*pSelectedFormat = availableFormats[0];
		return true;
	}

	Bool8 PickPresentationMode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, Bool8 vSync, VkPresentModeKHR* pSelectedPresentMode)
	{
		Array<VkPresentModeKHR> availablePresentModes;

		if (!EnumeratePresentModes(physicalDevice, surface, availablePresentModes))
		{
			Log.Error("Failed to pick surface format: Unable to enumerate present modes!");
			return false;
		}

		if (!vSync && availablePresentModes.Contains(VK_PRESENT_MODE_IMMEDIATE_KHR))
		{
			*pSelectedPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			return true;
		}

		else if (availablePresentModes.Contains(VK_PRESENT_MODE_MAILBOX_KHR))
		{
			*pSelectedPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			return true;
		}

		// VK_PRESENT_MODE_FIFO_KHR is guaranteed by the spec
		*pSelectedPresentMode = VK_PRESENT_MODE_FIFO_KHR;
		return true;
	}

	Bool8 VulkanSurface::CreateSurface(VkInstance instance, Window& window)
	{
		if (!CreateWin32VulkanSurface(instance, (Handle64)window.GetNativeHandle(), &mSurface))
		{
			Log.Critical("Failed to create vulkan surface: CreateWin32VulkanSurface failed!");
			return false;
		}

		return true;
	}

	Bool8 VulkanSurface::CreateSwapChain(VkInstance instance, VulkanDevice& device, VkSurfaceKHR surface, UInt32 bufferCount, UInt32 width, UInt32 height, Bool8 fullscreen, UInt32 vSync)
	{
		VkSurfaceFormatKHR	selectedFormat;
		VkPresentModeKHR	selectedPresentMode;

		const VkPhysicalDevice& physicalDevice = device.GetPhysicalDeviceHandle().GetPhysicalDeviceHandle();

		if (!PickSurfaceFormat(physicalDevice, surface, &selectedFormat))
		{
			Log.Error("Failed to create vulkan swapchain: No suitable surface format was detected!");
			return false;
		}

		if (!PickPresentationMode(physicalDevice, surface, vSync, &selectedPresentMode))
		{
			Log.Error("Failed to create vulkan swapchain: No suitable present mode was detected!");
			return false;
		}

		if (selectedPresentMode != VK_PRESENT_MODE_IMMEDIATE_KHR)
		{
			mIsVSynced = true;
		}

		VkSurfaceCapabilitiesKHR surfaceCapabilites;
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilites) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan swapchain: Failed to query surface capabilites!");
			return false;
		}

		VkSurfaceTransformFlagBitsKHR preTransform = surfaceCapabilites.currentTransform;
		if (surfaceCapabilites.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR)
		{
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}

		VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR;
		if (surfaceCapabilites.supportedCompositeAlpha & VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR)
		{
			compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		}

		VkBool32 supportsPresent = false;
		if (vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, device.GetPresentQueue().GetFamilyIndex(), surface, &supportsPresent) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan swapchain: Specified device and queue do not support presentation!");
			return false;
		}

		UInt32 imageCount = bufferCount;
		if (bufferCount < surfaceCapabilites.minImageCount)
		{
			imageCount = surfaceCapabilites.minImageCount;
		}

		VkSwapchainCreateInfoKHR swapChainInfo = {};
		swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainInfo.surface = surface;
		swapChainInfo.minImageCount = imageCount;
		swapChainInfo.imageFormat = selectedFormat.format;
		swapChainInfo.imageColorSpace = selectedFormat.colorSpace;
		swapChainInfo.imageExtent = surfaceCapabilites.currentExtent;
		swapChainInfo.imageArrayLayers = 1;
		swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainInfo.preTransform = preTransform;
		swapChainInfo.compositeAlpha = compositeAlpha;
		swapChainInfo.presentMode = selectedPresentMode;
		swapChainInfo.clipped = VK_TRUE;
		swapChainInfo.oldSwapchain = VK_NULL_HANDLE;
		swapChainInfo.pNext = nullptr;

		/*
		VkSurfaceFullScreenExclusiveInfoEXT fullscreenInfo;
		if (device.SupportsFullscreen())
		{
		fullscreenInfo.sType = VK_STRUCTURE_TYPE_SURFACE_FULL_SCREEN_EXCLUSIVE_INFO_EXT;
		fullscreenInfo.fullScreenExclusive = fullscreen ? VK_FULL_SCREEN_EXCLUSIVE_ALLOWED_EXT : VK_FULL_SCREEN_EXCLUSIVE_DISALLOWED_EXT;

		swapChainInfo.pNext = &fullscreenInfo;

		mIsFullscreen = true;
		}
		*/

		if (vkCreateSwapchainKHR(device.GetDeviceHandle(), &swapChainInfo, nullptr, &mSwapChain) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan swapchain: vkCreateSwapchainKHR failed!");
			return false;
		}

		if (!RetrieveSwapChainImages(device.GetDeviceHandle(), mSwapChain, mImages))
		{
			Log.Error("Failed to create vulkan swapchain: Failed to retrieve swap chain images!");
			return false;
		}

		mWidth = swapChainInfo.imageExtent.width;
		mHeight = swapChainInfo.imageExtent.height;
		mFormat = selectedFormat.format;

		return true;
	}

	Bool8 VulkanSurface::CreateImageViews(VulkanDevice& device, VkFormat format)
	{
		mImageViews.Resize(mImages.Size());

		UInt32 imageIndex = 0;
		for (const VkImage& image : mImages)
		{
			mImageViews[imageIndex] = VulkanImageView(device, mImages[imageIndex], VK_IMAGE_VIEW_TYPE_2D, format, VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1);
			++imageIndex;
		}

		return true;
	}

	Bool8 VulkanSurface::CreateSemaphores()
	{
		mImageAcquiredSemaphores.Resize(mImages.Size());
		mRenderCompleteSemaphores.Resize(mImages.Size());
		mImageFences.Resize(mImages.Size());

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		semaphoreInfo.pNext = nullptr;
		semaphoreInfo.flags = 0;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.pNext = nullptr;
		fenceInfo.flags = 0;

		for (UInt32 i = 0; i < mImages.Size(); i++)
		{
			vkCreateSemaphore(mpDevice->GetDeviceHandle(), &semaphoreInfo, nullptr, &mImageAcquiredSemaphores[i]);
			vkCreateSemaphore(mpDevice->GetDeviceHandle(), &semaphoreInfo, nullptr, &mRenderCompleteSemaphores[i]);
			vkCreateFence(mpDevice->GetDeviceHandle(), &fenceInfo, nullptr, &mImageFences[i]);
		}

		mImageIndex = -1;

		return true;
	}

	VulkanSurface::VulkanSurface(VkInstance instance, Window& window, VulkanDevice& device, UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen)
		: VulkanSurface()
	{
		mpWindow = &window;
		mpDevice = &device;

		if (
			CreateSurface(instance, window) &&
			CreateSwapChain(instance, device, mSurface, 2, width, height, fullscreen, vSync) &&
			CreateImageViews(device, mFormat) &&
			CreateSemaphores()
			)
		{
			mIsValidSurface = true;
		}
	}

	Int32 VulkanSurface::AcquireNextImageIndex()
	{
		UInt32 imageIndex;
		if (vkAcquireNextImageKHR(mpDevice->GetDeviceHandle(), mSwapChain, UINT64_MAX,
			mImageAcquiredSemaphores[mImageIndex], VK_NULL_HANDLE, &imageIndex) != VK_SUCCESS)
		{
			Log.Error("Error acquiring next image index: vkAcquireNextImageKHR failed!");
			return -1;
		}

		mImageIndex = imageIndex;
	}
	
}

