#include "Win32Vulkan.h"

#include "log/Log.h"
#include "vulkan/vulkan_win32.h"

namespace Quartz
{
    HGFXSurface Win32VulkanContext::CreateSurface(Window& window, UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen)
    {
        VkSurfaceKHR surface;

		Win32Window* pWindow = static_cast<Win32Window*>(&window);

        VkWin32SurfaceCreateInfoKHR win32SurfaceInfo = {};
        win32SurfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        win32SurfaceInfo.hinstance = GetModuleHandle(nullptr);
        win32SurfaceInfo.hwnd = (HWND)window.GetNativeHandle();

        VkResult result = vkCreateWin32SurfaceKHR(mVkInstance, &win32SurfaceInfo, nullptr, &surface);

        if (result != VK_SUCCESS)
        {
            Log.Critical("vkCreateWin32SurfaceKHR failed to create surface!!!");
        }

		Win32VulkanSurface* pSurface = new Win32VulkanSurface;
		pSurface->surface = surface;
		pSurface->bounds = { 0, 0, static_cast<Int32>(width), static_cast<Int32>(height) };
		pSurface->pWindow = pWindow;

        return HGFXSurface(pSurface);
    }

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

    HGFXSwapchain Win32VulkanContext::CreateSwapchain(HGFXSurface surface, Window& window, UInt32 bufferCount, UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen)
    {
		VkSurfaceFormatKHR selectedFormat;
		VkPresentModeKHR selectedPresentMode;

		Win32VulkanSurface* pWin32VkSurface = static_cast<Win32VulkanSurface*>(surface);
		VkSurfaceKHR vkSurface = pWin32VkSurface->surface;
		VkSwapchainKHR vkSwapchain;

		const VkPhysicalDevice& physicalDevice = mpDevice->GetPhysicalDevice().GetPhysicalDeviceHandle();

		if (!PickSurfaceFormat(physicalDevice, vkSurface, &selectedFormat))
		{
			Log.Error("Failed to create vulkan swapchain: No suitable surface format was detected!");
			return HGFX_NULL_HANDLE;
		}

		if (!PickPresentationMode(physicalDevice, vkSurface, vSync, &selectedPresentMode))
		{
			Log.Error("Failed to create vulkan swapchain: No suitable present mode was detected!");
			return HGFX_NULL_HANDLE;
		}

		VkSurfaceCapabilitiesKHR surfaceCapabilites;
		if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, vkSurface, &surfaceCapabilites) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan swapchain: Failed to query surface capabilites!");
			return HGFX_NULL_HANDLE;
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
		if (vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, mpDevice->GetPresentQueue().GetFamilyIndex(), vkSurface, &supportsPresent) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan swapchain: Specified device and queue do not support presentation!");
			return HGFX_NULL_HANDLE;
		}

		UInt32 imageCount = bufferCount;
		if (bufferCount < surfaceCapabilites.minImageCount)
		{
			imageCount = surfaceCapabilites.minImageCount;
		}

		VkSwapchainCreateInfoKHR swapChainInfo = {};
		swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainInfo.surface = vkSurface;
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

		if (vkCreateSwapchainKHR(mpDevice->GetDeviceHandle(), &swapChainInfo, nullptr, &vkSwapchain) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan swapchain: vkCreateSwapchainKHR failed!");
			return HGFX_NULL_HANDLE;
		}

		VulkanSwapchain* pSwapchain = new VulkanSwapchain();

		Array<VkImage> rawImages;
		UInt32 swapChainImageCount = 0;

		vkGetSwapchainImagesKHR(mpDevice->GetDeviceHandle(), vkSwapchain, &swapChainImageCount, nullptr);
		rawImages.Resize(swapChainImageCount);
		vkGetSwapchainImagesKHR(mpDevice->GetDeviceHandle(), vkSwapchain, &swapChainImageCount, rawImages.Data());

		pSwapchain->images.Resize(swapChainImageCount);
		pSwapchain->imageViews.Resize(swapChainImageCount);
		pSwapchain->imageAcquiredSemaphores.Resize(swapChainImageCount);
		pSwapchain->imageCompleteSemaphores.Resize(swapChainImageCount);
		pSwapchain->imageFences.Resize(swapChainImageCount);

		for (UInt32 i = 0; i < swapChainImageCount; i++)
		{
			pSwapchain->images[i].vkImage = rawImages[i];
			pSwapchain->images[i].vkFormat = selectedFormat.format;
			pSwapchain->images[i].vkUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			pSwapchain->images[i].vkImageType = VK_IMAGE_TYPE_2D;
			pSwapchain->images[i].pMemory = nullptr;
			pSwapchain->images[i].width = swapChainInfo.imageExtent.width;
			pSwapchain->images[i].height = swapChainInfo.imageExtent.height;
			pSwapchain->images[i].depth = 1;
			pSwapchain->images[i].mipLevels = 1;
			pSwapchain->images[i].layers = 1;

			CreateImageViewImpl(&pSwapchain->imageViews[i].vkImageView, rawImages[i], VK_IMAGE_VIEW_TYPE_2D, 
				VK_IMAGE_ASPECT_COLOR_BIT, selectedFormat.format, 0, 1, 0, 1);

			VulkanImageView* pImageView = new VulkanImageView();
			pSwapchain->imageViews[i].vkFormat = selectedFormat.format;
			pSwapchain->imageViews[i].vkViewType = VK_IMAGE_VIEW_TYPE_2D;
			pSwapchain->imageViews[i].vkAspects = VK_IMAGE_ASPECT_COLOR_BIT;
			pSwapchain->imageViews[i].mipStart = 0;
			pSwapchain->imageViews[i].mipLevels = 1;
			pSwapchain->imageViews[i].layerStart = 0;
			pSwapchain->imageViews[i].layers = 1;

			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			semaphoreInfo.pNext = nullptr;
			semaphoreInfo.flags = 0;

			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.pNext = nullptr;
			fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT; // Start signaled

			vkCreateSemaphore(mpDevice->GetDeviceHandle(), &semaphoreInfo, nullptr, &pSwapchain->imageAcquiredSemaphores[i]);
			vkCreateSemaphore(mpDevice->GetDeviceHandle(), &semaphoreInfo, nullptr, &pSwapchain->imageCompleteSemaphores[i]);
			vkCreateFence(mpDevice->GetDeviceHandle(), &fenceInfo, nullptr, &pSwapchain->imageFences[i]);
		}

		pSwapchain->vkSwapchain = vkSwapchain;
		pSwapchain->vkFormat = selectedFormat.format;
		pSwapchain->imageCount = swapChainImageCount;
		pSwapchain->width = swapChainInfo.imageExtent.width;
		pSwapchain->height = swapChainInfo.imageExtent.height;
		pSwapchain->imageIndex = 0;

        return HGFXSwapchain(pSwapchain);
    }
}

