#include "VulkanWin32.h"

#include "Win32.h"
#include "vulkan\vulkan_win32.h"

#include "gfx\GFXPlatform.h"
#include "Vulkan.h"
#include "VulkanUtil.h"
//#include "io\//Log.h"

namespace Quartz
{
	/********************************************************************************************/


	Bool8 VulkanWin32GFXModule::InitModuleImpl(const Char** pFailMessage)
	{
		return true;
	}


	/********************************************************************************************/


	Bool8 VulkanWin32GFXModule::DestroyModuleImpl(const Char** pFailMessage)
	{
		return true;
	}


	/********************************************************************************************/


	const Char* VulkanWin32GFXModule::GetModuleNameImpl() const
	{
		return "VulkanWin32";
	}


	/********************************************************************************************/


	GFXSurface* VulkanWin32GFXModule::CreateSurface(GFXModule* pGFX, GFXDevice* pDevice, GFXSurfaceInfo info)
	{
		if (!pGFX || !pDevice || !info.pWindow)
		{
			//Log.Critical("Failed to create surface: Null or invalid parameters!");
			return NULL;
		}

		VkSurfaceKHR surface;
		VkSurfaceCapabilitiesKHR capabilities;
		Array<VkSurfaceFormatKHR> surfaceFormats;
		Array<VkPresentModeKHR> presentModes;

		uint32_t formatCount;
		uint32_t presentModeCount;

		VkInstance instance = static_cast<VulkanGFXModule*>(pGFX)->GetVkInstance();
		VkPhysicalDevice physicalDevice = static_cast<VulkanGFXPhysicalDevice*>(pDevice->GetPhysicalDevice())->GetVkPhysicalDevice();

		VkWin32SurfaceCreateInfoKHR createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		createInfo.hwnd = (HWND)info.pWindow->GetNativeHandle();
		createInfo.hinstance = GetModuleHandle(nullptr);

		VkBool32 supported = VK_FALSE;

		if (vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface) != VK_SUCCESS)
		{
			//Log.Critical("Failed to create surface: vkCreateWin32SurfaceKHR failed!");
			return NULL;
		}

		if (vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, 0, surface, &supported) != VK_SUCCESS || !supported)
		{
			//Log.Critical("Failed to create surface: Device does not support surfaces!");
			return NULL;
		}

		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);
		surfaceFormats.Resize(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, surfaceFormats.Data());

		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
		presentModes.Resize(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.Data());

		SetDebugMarkerObjectName(pDevice, VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT, (UInt64)surface,
			(String("[Surface] ") + info.debugName).Str());

		return new VulkanGFXSurface(surface, capabilities, surfaceFormats, presentModes);;
	}


	/********************************************************************************************/


	void VulkanWin32GFXModule::DestroySurface(GFXModule* pGFX, GFXSurface* pSurface)
	{
		VkInstance instance = static_cast<VulkanGFXModule*>(pGFX)->GetVkInstance();
		VkSurfaceKHR surface = static_cast<VulkanGFXSurface*>(pSurface)->GetVkSurface();

		vkDestroySurfaceKHR(instance, surface, VK_NULL_HANDLE);

		delete pSurface;
	}


	/********************************************************************************************/


	VulkanGFXSurface::VulkanGFXSurface(VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR capibilities,
		const Array<VkSurfaceFormatKHR>& surfaceFormats, const Array<VkPresentModeKHR>& presentModes)
	{
		mVkSurface				= surface;
		mVkSurfaceCapabilities	= capibilities;
		mVkSurfaceFormats		= surfaceFormats;
		mVkPresentModes			= presentModes;
		mWidth					= capibilities.currentExtent.width;
		mHeight					= capibilities.currentExtent.height;
	}


	/********************************************************************************************/


	VkSurfaceFormatKHR ChooseBestSurfaceFormat(GFXSurface* pSurface)
	{
		//TODO: allow 10-bit HDR

		const Array<VkSurfaceFormatKHR>& surfaceFormats = static_cast<VulkanGFXSurface*>(pSurface)->GetVkSurfaceFormats();

		for (const VkSurfaceFormatKHR format : const_cast<Array<VkSurfaceFormatKHR>&>(surfaceFormats))
		{
			if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				return format;
			}
		}

		return surfaceFormats[0];
	}


	/********************************************************************************************/


	GFXSwapchain* VulkanWin32GFXModule::CreateSwapchain(GFXDevice* pDevice, GFXSurface* pSurface, GFXSwapchainInfo info)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();
		VkSurfaceKHR surface = static_cast<VulkanGFXSurface*>(pSurface)->GetVkSurface();
		VkSurfaceCapabilitiesKHR capabilities = static_cast<VulkanGFXSurface*>(pSurface)->GetVkSurfaceCapibilities();

		VkPresentModeKHR presentMode;
		VkSharingMode sharingMode;

		VkSurfaceFormatKHR surfaceFormat = ChooseBestSurfaceFormat(pSurface);

		switch (info.presentMode)
		{
			case GFX_PRESENT_MODE_IMMEDIATE:			presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR; break;
			case GFX_PRESENT_MODE_VSYNC:				presentMode = VK_PRESENT_MODE_FIFO_KHR; break;
			case GFX_PRESENT_MODE_TRIPPLE_BUFFERED:		presentMode = VK_PRESENT_MODE_MAILBOX_KHR; break;
			default:									presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
		}

		switch (info.sharingMode)
		{
			case GFX_SHARING_MODE_EXCLUSIVE:	sharingMode = VK_SHARING_MODE_EXCLUSIVE; break;
			case GFX_SHARING_MODE_SHARED:		sharingMode = VK_SHARING_MODE_CONCURRENT; break;
			default:							sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		}

		VkSwapchainCreateInfoKHR swapchainInfo = {};
		swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainInfo.surface = surface;
		swapchainInfo.minImageCount = capabilities.minImageCount;
		swapchainInfo.imageFormat = surfaceFormat.format;
		swapchainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainInfo.imageExtent = capabilities.currentExtent;
		swapchainInfo.imageArrayLayers = info.imageLayers;
		swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		swapchainInfo.imageSharingMode = sharingMode;
		swapchainInfo.preTransform = capabilities.currentTransform;
		swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainInfo.presentMode = presentMode;
		swapchainInfo.clipped = info.imageLayers;
		swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

		VkSwapchainKHR swapchain;

		if (vkCreateSwapchainKHR(device, &swapchainInfo, nullptr, &swapchain) != VK_SUCCESS)
		{
			//Log.Critical("Failed to create Swapchain: vkCreateSwapchainKHR failed!");
			return NULL;
		}

		SetDebugMarkerObjectName(pDevice, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (UInt64)swapchain,
			(String("[Swapchain] ") + info.debugName).Str());

		return new VulkanGFXSwapchain(pDevice, pSurface, swapchain, surfaceFormat, info);
	}


	/********************************************************************************************/


	void VulkanWin32GFXModule::DestroySwapchain(GFXSwapchain* pSwapchain)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pSwapchain->GetParentDevice())->GetVkDevice();
		VkSwapchainKHR swapchain = static_cast<VulkanGFXSwapchain*>(pSwapchain)->GetVkSwapchain();

		vkDestroySwapchainKHR(device, swapchain, VK_NULL_HANDLE);
	}


	/********************************************************************************************/


	GFXImageArray VulkanWin32GFXModule::GetSwapChainImages(GFXSwapchain* pSwapchain)
	{
		VulkanGFXDevice* pVulkanDevice = static_cast<VulkanGFXDevice*>(pSwapchain->GetParentDevice());
		VulkanGFXSwapchain* pVulkanSwapchain = static_cast<VulkanGFXSwapchain*>(pSwapchain);
		VulkanGFXSurface* pVulkanSurface = static_cast<VulkanGFXSurface*>(pSwapchain->GetSurface());
		VkDevice device = static_cast<VulkanGFXDevice*>(pVulkanDevice)->GetVkDevice();
		VkSwapchainKHR swapchain = static_cast<VulkanGFXSwapchain*>(pSwapchain)->GetVkSwapchain();
		VkSurfaceFormatKHR format = pVulkanSwapchain->GetVkSurfaceFormat();
		VkSurfaceCapabilitiesKHR capabilities = pVulkanSurface->GetVkSurfaceCapibilities();

		GFXImageInfo imageInfo = {};
		imageInfo.width = capabilities.currentExtent.width;
		imageInfo.height = capabilities.currentExtent.height;
		imageInfo.format = GetGFXImageFormat(format.format);
		imageInfo.imageType = GFX_IMAGE_TYPE_2D;
		imageInfo.mipLevels = 1;
		imageInfo.multisamples = 1;
		imageInfo.sharingMode = pVulkanSwapchain->GetSharingMode();

		Array<VkImage> images;
		GFXImageArray imageArray;
		UInt32 imageCount;

		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, VK_NULL_HANDLE);
		images.Resize(imageCount);
		vkGetSwapchainImagesKHR(device, swapchain, &imageCount, images.Data());

		for (VkImage image : images)
		{
			//TODO: find a way to extract proper elements
			imageArray.PushBack(new VulkanGFXImage(pVulkanDevice, image, format.format, 
				VkDeviceMemory(), VkMemoryRequirements(), imageInfo));
		}

		return imageArray;
	}


	/********************************************************************************************/


	VulkanGFXSwapchain::VulkanGFXSwapchain(GFXDevice* pDevice, GFXSurface* pSurface, VkSwapchainKHR swapchain, 
		VkSurfaceFormatKHR surfaceFormat, GFXSwapchainInfo info)
	{
		mDebugName = info.debugName;
		mpParentDevice = pDevice;
		mpSurface = pSurface;
		mPresentMode = info.presentMode;
		mSharingMode = info.sharingMode;
		mImageLayers = info.imageLayers;
		mClipped = info.clipped;
		mVkSwapchain = swapchain;
		mVkSurfaceFormat = surfaceFormat;
	}


	/********************************************************************************************/
}

