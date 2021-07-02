#include "VulkanGraphics.h"

#include "VulkanViewport.h"
#include "VulkanRenderpass.h"
#include "VulkanFramebuffer.h"
#include "VulkanShader.h"
#include "VulkanPipeline.h"
#include "VulkanBuffer.h"
#include "VulkanUniform.h"

#include "VulkanUtil.h"

#include "Engine.h"
#include "log/Log.h"

namespace Quartz
{
	VulkanGraphics::VulkanGraphics()
		: Graphics({ L"Vulkan Graphics" }), 
		mvkInstance(), 
		mpDevice(nullptr)
	{
		// Nothing
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

	Bool8 PickSurfaceFormat(VkPhysicalDevice physicalDevice, VulkanSurface* pSurface, Bool8 preferHDR, VkSurfaceFormatKHR* pSelectedFormat)
	{
		Array<VkSurfaceFormatKHR> availableFormats;

		for (const VkSurfaceFormatKHR& format : pSurface->GetSupportedFormats())
		{
			if (preferHDR)
			{
				if (format.format == VK_FORMAT_A2B10G10R10_SNORM_PACK32 && format.colorSpace == VK_COLOR_SPACE_HDR10_ST2084_EXT)
				{
					*pSelectedFormat = format;
					return true;
				}
			}
			else
			{
				if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					*pSelectedFormat = format;
					return true;
				}
			}
		}

		// No prefered format found, use default
		*pSelectedFormat = availableFormats[0];

		return true;
	}

	Bool8 PickPresentationMode(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, Bool8 vSync, VkPresentModeKHR* pSelectedPresentMode)
	{
		Array<VkPresentModeKHR> availablePresentModes;

		if (!EnumeratePresentModes(physicalDevice, surface, availablePresentModes))
		{
			Log::Error("Failed to pick surface format: Unable to enumerate present modes!");
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

	VkResult VulkanGraphics::CreateVkImageView(VkImageView* pVkImageView, VkImage vkImage, 
		VkImageViewType vkImageViewType, VkImageAspectFlags vkAspectFlags, VkFormat vkFormat, 
		UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layers)
	{
		VkImageView vkImageView;

		VkImageViewCreateInfo vkImageViewInfo = {};
		vkImageViewInfo.sType		= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		vkImageViewInfo.flags		= 0;
		vkImageViewInfo.image		= vkImage;
		vkImageViewInfo.viewType	= vkImageViewType;
		vkImageViewInfo.format		= vkFormat;

		vkImageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		vkImageViewInfo.subresourceRange.aspectMask			= vkAspectFlags;
		vkImageViewInfo.subresourceRange.baseMipLevel		= mipStart;
		vkImageViewInfo.subresourceRange.levelCount			= mipLevels;
		vkImageViewInfo.subresourceRange.baseArrayLayer		= layerStart;
		vkImageViewInfo.subresourceRange.layerCount			= layers;

		VkResult result = vkCreateImageView(mpDevice->GetDeviceHandle(), &vkImageViewInfo, nullptr, &vkImageView);

		if (result != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan image view: vkCreateImageView failed!");
			return result;
		}

		*pVkImageView = vkImageView;

		return result;
	}

	VulkanSwapchain* VulkanGraphics::CreateSwapchain(VulkanSurface* pSurface, UInt32 bufferCount)
	{
		VkSwapchainKHR		vkSwapchain;
		VkSurfaceFormatKHR	selectedFormat;
		VkPresentModeKHR	selectedPresentMode;

		VkResult result;

		VkSurfaceKHR vkSurface = pSurface->GetVkSurface();

		const VkPhysicalDevice& physicalDevice = mpDevice->GetPhysicalDevice().GetPhysicalDeviceHandle();

		if (!PickSurfaceFormat(physicalDevice, pSurface, false, &selectedFormat))
		{
			Log::Error("Failed to create vulkan swapchain: No suitable surface format was detected!");
			return nullptr;
		}

		if (!PickPresentationMode(physicalDevice, vkSurface, false, &selectedPresentMode))
		{
			Log::Error("Failed to create vulkan swapchain: No suitable present mode was detected!");
			return nullptr;
		}

		VkSurfaceCapabilitiesKHR surfaceCapabilites = pSurface->GetCapibilites();

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
			Log::Error("Failed to create vulkan swapchain: Specified device and queue do not support presentation!");
			return nullptr;
		}

		UInt32 imageCount = bufferCount;

		if (bufferCount < surfaceCapabilites.minImageCount)
		{
			imageCount = surfaceCapabilites.minImageCount;
			Log::Warning("Swapchain only supports %d backbuffers. %d requested.", imageCount, bufferCount);
		}

		VkSwapchainCreateInfoKHR swapChainInfo = {};
		swapChainInfo.sType				= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainInfo.surface			= vkSurface;
		swapChainInfo.minImageCount		= imageCount;
		swapChainInfo.imageFormat		= selectedFormat.format;
		swapChainInfo.imageColorSpace	= selectedFormat.colorSpace;
		swapChainInfo.imageExtent		= surfaceCapabilites.currentExtent;
		swapChainInfo.imageArrayLayers	= 1;
		swapChainInfo.imageUsage		= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		swapChainInfo.imageSharingMode	= VK_SHARING_MODE_EXCLUSIVE;
		swapChainInfo.preTransform		= preTransform;
		swapChainInfo.compositeAlpha	= compositeAlpha;
		swapChainInfo.presentMode		= selectedPresentMode;
		swapChainInfo.clipped			= VK_TRUE;
		swapChainInfo.oldSwapchain		= VK_NULL_HANDLE;
		swapChainInfo.pNext				= nullptr;

		if (vkCreateSwapchainKHR(mpDevice->GetDeviceHandle(), &swapChainInfo, nullptr, &vkSwapchain) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan swapchain: vkCreateSwapchainKHR failed!");
			return nullptr;
		}

		UInt32			swapChainImageCount = 0;
		Array<VkImage>	swapchainImages;

		result = vkGetSwapchainImagesKHR(mpDevice->GetDeviceHandle(), vkSwapchain, &swapChainImageCount, nullptr);
		swapchainImages.Resize(swapChainImageCount);
		result = vkGetSwapchainImagesKHR(mpDevice->GetDeviceHandle(), vkSwapchain, &swapChainImageCount, swapchainImages.Data());

		if (result != VK_SUCCESS)
		{
			Log::Error("Failed to retrieve images from swapchain!");
			vkDestroySwapchainKHR(mpDevice->GetDeviceHandle(), vkSwapchain, VK_NULL_HANDLE);
			return nullptr;
		}

		Array<VulkanImage*>		images;
		Array<VulkanImageView*>	imageViews;
		Array<VkSemaphore>		imageAvailableSemaphores;
		Array<VkSemaphore>		imageCompleteSemaphores;
		Array<VkFence>			imageFences;
		Array<VkFence>			inFlightFences;

		images.Resize(swapChainImageCount);
		imageViews.Resize(swapChainImageCount);
		imageAvailableSemaphores.Resize(swapChainImageCount);
		imageCompleteSemaphores.Resize(swapChainImageCount);
		imageFences.Resize(swapChainImageCount);
		inFlightFences.Resize(swapChainImageCount, VK_NULL_HANDLE);

		for (UInt32 i = 0; i < swapChainImageCount; i++)
		{
			VkImage		vkImage		= swapchainImages[i];
			VkImageView vkImageView;

			ImageFormat imageFormat = VulkanUtil::VkFormatToImageFormat(selectedFormat.format);

			VulkanImage* pImage = new VulkanImage
			(
				vkImage,
				VK_NULL_HANDLE,
				selectedFormat.format,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				VK_IMAGE_TYPE_2D,
				IMAGE_TYPE_2D,
				swapChainInfo.imageExtent.width,
				swapChainInfo.imageExtent.height,
				1,
				1, 1,
				imageFormat,
				IMAGE_VIEW_USAGE_COLOR_ATTACHMENT
			);

			result = CreateVkImageView
			(
				&vkImageView,
				vkImage,
				VK_IMAGE_VIEW_TYPE_2D,
				VK_IMAGE_ASPECT_COLOR_BIT,
				selectedFormat.format,
				0, 1, 0, 1
			);

			if (result != VK_SUCCESS)
			{
				Log::Error("Failed to create swapchain: unable to create image views.");
				vkDestroySwapchainKHR(mpDevice->GetDeviceHandle(), vkSwapchain, VK_NULL_HANDLE);
				return nullptr;
			}

			VulkanImageView* pImageView = new VulkanImageView
			(
				vkImageView,
				VK_IMAGE_VIEW_TYPE_2D,
				VK_IMAGE_ASPECT_COLOR_BIT,
				pImage,
				IMAGE_VIEW_TYPE_2D,
				pImage->GetWidth(),
				pImage->GetHeight(),
				1,
				1, 0,
				1, 0,
				pImage->GetFormat(),
				IMAGE_VIEW_USAGE_COLOR_ATTACHMENT
			);

			VkSemaphoreTypeCreateInfo vkSemaphoreType = {};
			vkSemaphoreType.sType			= VK_STRUCTURE_TYPE_SEMAPHORE_TYPE_CREATE_INFO;
			vkSemaphoreType.semaphoreType	= VK_SEMAPHORE_TYPE_BINARY;
			vkSemaphoreType.initialValue	= 0;

			VkSemaphoreCreateInfo semaphoreInfo = {};
			semaphoreInfo.sType				= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			semaphoreInfo.pNext				= &vkSemaphoreType;
			semaphoreInfo.flags				= 0;

			VkFenceCreateInfo fenceInfo = {};
			fenceInfo.sType					= VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fenceInfo.pNext					= nullptr;
			fenceInfo.flags					= VK_FENCE_CREATE_SIGNALED_BIT; // Start signaled

			vkCreateSemaphore(mpDevice->GetDeviceHandle(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]);
			vkCreateSemaphore(mpDevice->GetDeviceHandle(), &semaphoreInfo, nullptr, &imageCompleteSemaphores[i]);
			vkCreateFence(mpDevice->GetDeviceHandle(), &fenceInfo, nullptr, &inFlightFences[i]);

			TransitionImage(pImage, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);

			images[i]		= pImage;
			imageViews[i]	= pImageView;
		}

		VulkanSwapchain* pVulkanSwapchain = new VulkanSwapchain
		(
			vkSwapchain, mpDevice, imageCount, images, imageViews, imageAvailableSemaphores,
			imageCompleteSemaphores, imageFences, inFlightFences
		);

		return pVulkanSwapchain;
	}

	void VulkanGraphics::TransitionImage(VulkanImage* pImage, VkImageLayout oldLayout, VkImageLayout newLayout)
	{
		// @TODO: Probably clean this up to not have to recreate command buffers

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool			= mpDevice->GetGraphicsCommandPoolHandle();
		allocInfo.commandBufferCount	= 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(mpDevice->GetDeviceHandle(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkImageMemoryBarrier barrier{};
		barrier.sType							= VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout						= oldLayout;
		barrier.newLayout						= newLayout;
		barrier.srcQueueFamilyIndex				= VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex				= VK_QUEUE_FAMILY_IGNORED;
		barrier.image							= pImage->GetVkImage();
		barrier.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel	= 0;
		barrier.subresourceRange.levelCount		= 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount		= 1;
		barrier.srcAccessMask					= 0;
		barrier.dstAccessMask					= 0;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
			newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			&& newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else if (newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR)
		{
			// @TODO: double check these values

			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else
		{
			Log::Critical("Failed to transition image: Invalid image layouts");
			vkEndCommandBuffer(commandBuffer);
			vkFreeCommandBuffers(mpDevice->GetDeviceHandle(), mpDevice->GetGraphicsCommandPoolHandle(), 1, &commandBuffer);
			return;
		}

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage,
			0, 0, nullptr, 0, nullptr, 1, &barrier);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(mpDevice->GetGraphicsQueue().GetQueueHandle(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(mpDevice->GetGraphicsQueue().GetQueueHandle());

		vkFreeCommandBuffers(mpDevice->GetDeviceHandle(), mpDevice->GetGraphicsCommandPoolHandle(), 1, &commandBuffer);
	}

	Context* VulkanGraphics::CreateContext(Surface* pSurface, Scene* pScene, Renderer* pRenderer, MultibufferType multibuffer)
	{
		VulkanSurface* pVulkanSurface = static_cast<VulkanSurface*>(pSurface);

		const UInt32 multibufferCount = (UInt32)multibuffer + 1;
		VulkanSwapchain* pSwapchain = CreateSwapchain(pVulkanSurface, multibufferCount);
		
		if (pSwapchain->GetBackbufferCount() != multibufferCount)
		{
			multibuffer = MultibufferType(pSwapchain->GetBackbufferCount() - 1);
		}
		
		Context* pContext = new VulkanContext(pSwapchain, pSurface, pScene, pRenderer, multibuffer);

		pRenderer->Setup(pContext);

		mViewports.PushBack(pContext);

		return pContext;
	}

	void VulkanGraphics::DestroyContext(Context* pContext)
	{
		// @TODO: DestroyContext()
	}

	Image* VulkanGraphics::CreateImage(ImageType type, UInt32 width, UInt32 height, UInt32 depth, 
		UInt32 layers, UInt32 mips, ImageFormat format, ImageUsages usages)
	{
		VkImage			vkImage;
		VkDeviceMemory	vkDeviceMemory;

		VkImageCreateInfo vkImageInfo = {};
		vkImageInfo.sType			= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		vkImageInfo.imageType		= VulkanUtil::ImageTypeToVkImageType(type);
		vkImageInfo.format			= VulkanUtil::ImageFormatToVkFormat(format);
		vkImageInfo.extent.width	= width;
		vkImageInfo.extent.height	= height;
		vkImageInfo.extent.depth	= depth;
		vkImageInfo.mipLevels		= mips;
		vkImageInfo.arrayLayers		= layers;
		vkImageInfo.tiling			= VK_IMAGE_TILING_OPTIMAL;
		vkImageInfo.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
		vkImageInfo.usage			= VulkanUtil::ImageUsagesToVkImageUsageFlags(usages);
		vkImageInfo.sharingMode		= VK_SHARING_MODE_EXCLUSIVE;
		vkImageInfo.samples			= VK_SAMPLE_COUNT_1_BIT;

		if (vkCreateImage(mpDevice->GetDeviceHandle(), &vkImageInfo, nullptr, &vkImage) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan image: vkCreateImage failed!");
			return nullptr;
		}

		VkMemoryRequirements vkMemRequirements;
		vkGetImageMemoryRequirements(mpDevice->GetDeviceHandle(), vkImage, &vkMemRequirements);

		UInt32 memoryType = VulkanUtil::FindCompatableMemoryType(mpDevice, 
			vkMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize		= vkMemRequirements.size;
		allocateInfo.memoryTypeIndex	= memoryType;

		if (vkAllocateMemory(mpDevice->GetDeviceHandle(), &allocateInfo, nullptr, &vkDeviceMemory) != VK_SUCCESS)
		{
			Log::Critical("Failed to allocate device memory: vkAllocateMemory failed!");
			return nullptr;
		}

		if (vkBindImageMemory(mpDevice->GetDeviceHandle(), vkImage, vkDeviceMemory, 0) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan buffer object: vkBindBufferMemory failed!");

			vkDestroyImage(mpDevice->GetDeviceHandle(), vkImage, nullptr);
			vkFreeMemory(mpDevice->GetDeviceHandle(), vkDeviceMemory, VK_NULL_HANDLE);

			return nullptr;
		}

		return new VulkanImage(
			vkImage, 
			vkDeviceMemory, 
			vkImageInfo.format, 
			vkImageInfo.usage, 
			vkImageInfo.imageType, 
			type,
			width, height, depth, 
			layers, mips, format, usages);
	}

	void VulkanGraphics::DestroyImage(Image* pImage)
	{
		VulkanImage* pVulkanImage = static_cast<VulkanImage*>(pImage);

		if (pVulkanImage->GetVkDeviceMemory() == VK_NULL_HANDLE)
		{
			// Likely a swapchain image, dont destroy
			return;
		}

		vkDestroyImage(mpDevice->GetDeviceHandle(), pVulkanImage->GetVkImage(), nullptr);
		vkFreeMemory(mpDevice->GetDeviceHandle(), pVulkanImage->GetVkDeviceMemory(), VK_NULL_HANDLE);

		delete pVulkanImage;
	}

	ImageView* VulkanGraphics::CreateImageView(Image* pImage, ImageViewType type, 
		UInt32 width, UInt32 height, UInt32 depth, 
		UInt32 layers, UInt32 layerStart, UInt32 mips, UInt32 mipStart, 
		ImageFormat format, ImageViewUsage usage)
	{
		VkImageView		vkImageView		= VK_NULL_HANDLE;
		VulkanImage*	pVulkanImage	= static_cast<VulkanImage*>(pImage);

		VkImageViewCreateInfo vkImageViewInfo = {};
		vkImageViewInfo.sType		= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		vkImageViewInfo.flags		= 0;
		vkImageViewInfo.image		= pVulkanImage->GetVkImage();
		vkImageViewInfo.viewType	= VulkanUtil::ImageViewTypeToVkImageViewType(type);
		vkImageViewInfo.format		= pVulkanImage->GetVkFormat();

		vkImageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		vkImageViewInfo.subresourceRange.aspectMask		= VulkanUtil::ImageUsageToVkImageAspects(usage);
		vkImageViewInfo.subresourceRange.baseMipLevel	= mipStart;
		vkImageViewInfo.subresourceRange.levelCount		= mips;
		vkImageViewInfo.subresourceRange.baseArrayLayer = layerStart;
		vkImageViewInfo.subresourceRange.layerCount		= layers;

		if (vkCreateImageView(mpDevice->GetDeviceHandle(), &vkImageViewInfo, nullptr, &vkImageView) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan image view: vkCreateImageView failed!");
			return nullptr;
		}

		return new VulkanImageView(
			vkImageView, 
			vkImageViewInfo.viewType, 
			vkImageViewInfo.subresourceRange.aspectMask, 
			pImage, 
			type, 
			width, height, depth, 
			layers, layerStart,
			mips, mipStart, 
			format, usage);
	}

	void VulkanGraphics::DestroyImageView(ImageView* pImageView)
	{

	}

	Buffer* VulkanGraphics::CreateBuffer(UInt32 sizeBytes, BufferUsages usages, BufferAccess access)
	{
		VkBuffer				vkBuffer;
		VkDeviceMemory			vkMemory;
		VkMemoryRequirements	vkMemRequirements;
		VkMemoryPropertyFlags	vkMemoryProperties;
		VkBufferUsageFlags		vkBufferUsages;

		vkBufferUsages		= VulkanUtil::BufferUsagesToVkBufferUsageFlags(usages);
		vkMemoryProperties	= VulkanUtil::BufferAccessToVkMemoryPropertyFlags(access);

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType		= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size			= sizeBytes;
		bufferInfo.usage		= vkBufferUsages;
		bufferInfo.sharingMode	= VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(mpDevice->GetDeviceHandle(), &bufferInfo, nullptr, &vkBuffer) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan buffer object: vkCreateBuffer failed!");
			return nullptr;
		}

		vkGetBufferMemoryRequirements(mpDevice->GetDeviceHandle(), vkBuffer, &vkMemRequirements);

		if (vkMemRequirements.size < sizeBytes)
		{
			Log::Warning("Warning creating buffer: Requested %d bytes, but only %d were allocated!");
		}

		UInt32 memoryType = VulkanUtil::FindCompatableMemoryType(mpDevice,
			vkMemRequirements.memoryTypeBits, vkMemoryProperties);

		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize		= vkMemRequirements.size;
		allocateInfo.memoryTypeIndex	= memoryType;

		if (vkAllocateMemory(mpDevice->GetDeviceHandle(), &allocateInfo, nullptr, &vkMemory) != VK_SUCCESS)
		{
			Log::Critical("Failed to allocate device memory: vkAllocateMemory failed!");
			vkDestroyBuffer(mpDevice->GetDeviceHandle(), vkBuffer, VK_NULL_HANDLE); 
			return nullptr;
		}

		if (vkBindBufferMemory(mpDevice->GetDeviceHandle(), vkBuffer, vkMemory, 0) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan buffer object: vkBindBufferMemory failed!");

			vkDestroyBuffer(mpDevice->GetDeviceHandle(), vkBuffer, VK_NULL_HANDLE);
			vkFreeMemory(mpDevice->GetDeviceHandle(), vkMemory, VK_NULL_HANDLE);

			return nullptr;
		}

		return new VulkanBuffer(vkBuffer, mpDevice, vkBufferUsages, vkMemory, 
			vkMemoryProperties, sizeBytes, usages, access);
	}

	void VulkanGraphics::DestroyBuffer(Buffer* pBuffer)
	{
		VulkanBuffer* pVulkanBuffer = static_cast<VulkanBuffer*>(pBuffer);

		vkDestroyBuffer(mpDevice->GetDeviceHandle(), pVulkanBuffer->GetVkBuffer(), VK_NULL_HANDLE);
		vkFreeMemory(mpDevice->GetDeviceHandle(), pVulkanBuffer->GetVkMemory(), VK_NULL_HANDLE);
	}

	Uniform* VulkanGraphics::CreateUniform(UniformType type, UInt32 elementSize, UInt32 elementCount, UniformFlags flags)
	{
		VulkanUniform* pUniform = new VulkanUniform(mpDevice, type, elementSize, elementCount, flags);
		pUniform->BuildBuffers(3); // TODO: HARDCODED
		return pUniform;
	}

	void VulkanGraphics::DestroyUniform(Uniform* pUniform)
	{

	}

	Renderpass* VulkanGraphics::CreateRenderpass(const String& name, const Array<Attachment>& attachments, const Array<Subpass>& subpasses)
	{
		VkRenderPass vkRenderPass;

		Array<VkAttachmentDescription>		attachmentDescriptions;
		Array<VkSubpassDescription>			subpassDescriptions;
		Array<Array<VkAttachmentReference>> subpassColorAttachmentReferences;
		Array<Array<VkAttachmentReference>> subpassInputAttachmentReferences;
		Array<VkAttachmentReference>		subpassDepthAttachmentReferences;
		Array<VkSubpassDependency>			subpassDependancies;

		subpassColorAttachmentReferences.Resize(subpasses.Size());
		subpassInputAttachmentReferences.Resize(subpasses.Size());
		subpassDepthAttachmentReferences.Resize(subpasses.Size());

		for (Attachment& attachment : attachments)
		{
			VkAttachmentDescription vkAttachmentDescription = {};
			vkAttachmentDescription.flags	= 0;
			vkAttachmentDescription.format	= VulkanUtil::ImageFormatToVkFormat(attachment.GetFormat());
			vkAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;

			if (attachment.GetType() == ATTACHMENT_SWAPCHAIN)
			{
				vkAttachmentDescription.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
				vkAttachmentDescription.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
				vkAttachmentDescription.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vkAttachmentDescription.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
				vkAttachmentDescription.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
				vkAttachmentDescription.finalLayout		= VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			}
			else if (attachment.GetType() == ATTACHMENT_DEPTH)
			{
				vkAttachmentDescription.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
				vkAttachmentDescription.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
				vkAttachmentDescription.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vkAttachmentDescription.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
				vkAttachmentDescription.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
				vkAttachmentDescription.finalLayout		= VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
			}
			else if (attachment.GetType() == ATTACHMENT_DEPTH_STENCIL)
			{
				vkAttachmentDescription.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
				vkAttachmentDescription.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
				vkAttachmentDescription.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vkAttachmentDescription.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_STORE;
				vkAttachmentDescription.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
				vkAttachmentDescription.finalLayout		= VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			}
			else if (attachment.GetType() == ATTACHMENT_STENCIL)
			{
				vkAttachmentDescription.loadOp			= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vkAttachmentDescription.storeOp			= VK_ATTACHMENT_STORE_OP_DONT_CARE;
				vkAttachmentDescription.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vkAttachmentDescription.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_STORE;
				vkAttachmentDescription.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
				vkAttachmentDescription.finalLayout		= VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
			}
			else
			{
				vkAttachmentDescription.loadOp			= VK_ATTACHMENT_LOAD_OP_CLEAR;
				vkAttachmentDescription.storeOp			= VK_ATTACHMENT_STORE_OP_STORE;
				vkAttachmentDescription.stencilLoadOp	= VK_ATTACHMENT_LOAD_OP_DONT_CARE;
				vkAttachmentDescription.stencilStoreOp	= VK_ATTACHMENT_STORE_OP_DONT_CARE;
				vkAttachmentDescription.initialLayout	= VK_IMAGE_LAYOUT_UNDEFINED;
				vkAttachmentDescription.finalLayout		= VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			}

			attachmentDescriptions.PushBack(vkAttachmentDescription);
		}

		UInt32 subpassIndex = 0;
		for (Subpass& subpass : subpasses)
		{
			Array<VkAttachmentReference>&	inputReferences			= subpassInputAttachmentReferences[subpassIndex];
			Array<VkAttachmentReference>&	colorReferences			= subpassColorAttachmentReferences[subpassIndex];
			VkAttachmentReference&			depthStencilReference	= subpassDepthAttachmentReferences[subpassIndex];

			for (UInt32 attachmentId : subpass.GetAttachments())
			{
				// @TODO: index check!!!
				const Attachment& attachment = attachments[attachmentId];

				VkAttachmentReference vkAttachmentReference = {};
				vkAttachmentReference.attachment = attachmentId;

				if (attachment.GetType() == ATTACHMENT_SWAPCHAIN || 
					attachment.GetType() == ATTACHMENT_COLOR)
				{
					vkAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
					colorReferences.PushBack(vkAttachmentReference);
				}
				else if (attachment.GetType() == ATTACHMENT_DEPTH || 
					attachment.GetType() == ATTACHMENT_STENCIL || 
					attachment.GetType() == ATTACHMENT_DEPTH_STENCIL)
				{
					// @TODO: check for more than one depth/stencil
					vkAttachmentReference.layout = attachmentDescriptions[attachmentId].finalLayout;
					depthStencilReference = vkAttachmentReference;
				}
			}

			VkSubpassDescription vkSubpassDescription = {};
			vkSubpassDescription.flags						= 0;
			vkSubpassDescription.pipelineBindPoint			= VK_PIPELINE_BIND_POINT_GRAPHICS;
			vkSubpassDescription.inputAttachmentCount		= 0;
			vkSubpassDescription.pInputAttachments			= nullptr;
			vkSubpassDescription.colorAttachmentCount		= colorReferences.Size();
			vkSubpassDescription.pColorAttachments			= colorReferences.Data();
			vkSubpassDescription.pResolveAttachments		= 0;
			vkSubpassDescription.pDepthStencilAttachment	= &depthStencilReference;
			vkSubpassDescription.preserveAttachmentCount	= 0;
			vkSubpassDescription.pPreserveAttachments		= nullptr;

			subpassDescriptions.PushBack(vkSubpassDescription);
			
			VkSubpassDependency vkSubpassDependancy = {};
			vkSubpassDependancy.srcSubpass		= (subpassIndex - 1);
			vkSubpassDependancy.dstSubpass		= subpassIndex;
			vkSubpassDependancy.srcStageMask	= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			vkSubpassDependancy.dstStageMask	= VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			vkSubpassDependancy.srcAccessMask	= VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			vkSubpassDependancy.dstAccessMask	= VK_ACCESS_SHADER_READ_BIT;
			vkSubpassDependancy.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
			
			if (subpassIndex == 0)
			{
				vkSubpassDependancy.srcSubpass		= VK_SUBPASS_EXTERNAL;
				vkSubpassDependancy.srcStageMask	= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				vkSubpassDependancy.dstStageMask	= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
				vkSubpassDependancy.srcAccessMask	= VK_ACCESS_MEMORY_READ_BIT;
				vkSubpassDependancy.dstAccessMask	= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			}

			if (subpassIndex == subpasses.Size())
			{
				vkSubpassDependancy.dstSubpass		= VK_SUBPASS_EXTERNAL;
				vkSubpassDependancy.dstStageMask	= VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				vkSubpassDependancy.srcAccessMask	= VK_ACCESS_MEMORY_READ_BIT;
				vkSubpassDependancy.dstAccessMask	= VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			}

			subpassDependancies.PushBack(vkSubpassDependancy);

			subpassIndex++;
		}
		
		VkRenderPassCreateInfo vkRenderPassCreateInfo = {};
		vkRenderPassCreateInfo.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		vkRenderPassCreateInfo.flags			= 0;
		vkRenderPassCreateInfo.pNext			= nullptr;
		vkRenderPassCreateInfo.attachmentCount	= attachmentDescriptions.Size();
		vkRenderPassCreateInfo.pAttachments		= attachmentDescriptions.Data();
		vkRenderPassCreateInfo.subpassCount		= subpassDescriptions.Size();
		vkRenderPassCreateInfo.pSubpasses		= subpassDescriptions.Data();
		vkRenderPassCreateInfo.dependencyCount	= subpassDependancies.Size();
		vkRenderPassCreateInfo.pDependencies	= subpassDependancies.Data();

		if (vkCreateRenderPass(mpDevice->GetDeviceHandle(), &vkRenderPassCreateInfo, nullptr, &vkRenderPass) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan render pass: vkCreateRenderPass failed!");
			return nullptr;
		}

		return new VulkanRenderpass(vkRenderPass, name, attachments, subpasses);
	}

	void VulkanGraphics::DestroyRenderpass(Renderpass* pRenderpass)
	{
		VulkanRenderpass* pVulkanRenderpass = static_cast<VulkanRenderpass*>(pRenderpass);
		vkDestroyRenderPass(mpDevice->GetDeviceHandle(), pVulkanRenderpass->GetVkRenderPass(), VK_NULL_HANDLE);
		delete pVulkanRenderpass;
	}

	Framebuffer* VulkanGraphics::CreateFramebuffer(Renderpass* pRenderpass, Context* pContext, UInt32 width, UInt32 height)
	{
		VulkanRenderpass*	pVulkanRenderpass	= static_cast<VulkanRenderpass*>(pRenderpass);
		VulkanContext*		pVulkanContext		= static_cast<VulkanContext*>(pContext);

		Array<VkFramebuffer>	vkFramebuffers;
		Array<Image*>			images;
		Array<ImageView*>		imageViews;
		Array<VkImageView>		vkImageViews;

		UInt32 swapchainIndex = 0;
		UInt32 attachmentIndex = 0;

		for (Attachment& attachment : pRenderpass->GetAttachments())
		{
			ImageFormat		format	= attachment.GetFormat();
			ImageUsages		usages	= IMAGE_USAGE_INVALID;
			ImageViewUsage	usage	= IMAGE_VIEW_USAGE_INVALID;

			if (attachment.GetType() == ATTACHMENT_SWAPCHAIN)
			{
				imageViews.PushBack(nullptr);
				vkImageViews.PushBack(nullptr);
				swapchainIndex = attachmentIndex;
				continue;
			}

			switch (attachment.GetType())
			{
				case ATTACHMENT_COLOR:
				{
					usages	= IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
					usage	= IMAGE_VIEW_USAGE_COLOR_ATTACHMENT;
					break;
				}
				case ATTACHMENT_DEPTH:
				{
					usages	= IMAGE_USAGE_DEPTH_ATTACHMENT_BIT;
					usage	= IMAGE_VIEW_USAGE_DEPTH_ATTACHMENT;
					break;
				}
				case ATTACHMENT_STENCIL:
				{
					usages	= IMAGE_USAGE_STENCIL_ATTACHMENT_BIT;
					usage	= IMAGE_VIEW_USAGE_STENCIL_ATTACHMENT;
					break;
				}
				case ATTACHMENT_DEPTH_STENCIL:
				{
					usages	= IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
					usage	= IMAGE_VIEW_USAGE_DEPTH_STENCIL_ATTACHMENT;
					break;
				}
			}

			Image* pAttachmentImage = CreateImage(IMAGE_TYPE_2D, 
				width, height, 1, 1, 1, format, usages);

			ImageView* pAttachmentView = CreateImageView(pAttachmentImage, IMAGE_VIEW_TYPE_2D, 
				width, height, 1, 1, 0, 1, 0, format, usage);
		
			images.PushBack(pAttachmentImage);
			imageViews.PushBack(pAttachmentView);
			vkImageViews.PushBack(static_cast<VulkanImageView*>(pAttachmentView)->GetVkImageView());

			attachmentIndex++;
		}

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType			= VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass		= pVulkanRenderpass->GetVkRenderPass();
		framebufferInfo.attachmentCount = vkImageViews.Size();
		framebufferInfo.pAttachments	= vkImageViews.Data();
		framebufferInfo.width			= width;
		framebufferInfo.height			= height;
		framebufferInfo.layers			= 1;

		//TODO: this should not be decieded here
		vkFramebuffers.Resize(pVulkanContext->GetMultibufferCount());

		for (UInt32 i = 0; i < pVulkanContext->GetMultibufferCount(); i++)
		{
			vkImageViews[swapchainIndex] = pVulkanContext->GetSwapchain()->GetImageViews()[i]->GetVkImageView();

			if (vkCreateFramebuffer(mpDevice->GetDeviceHandle(), &framebufferInfo, VK_NULL_HANDLE, &vkFramebuffers[i]) != VK_SUCCESS)
			{
				Log::Error("Failed to create vulkan frame buffer: vkCreateFramebuffer failed!");

				for (UInt32 j = 0; j < i; j++)
				{
					// Destroy and previously created
					vkDestroyFramebuffer(mpDevice->GetDeviceHandle(), vkFramebuffers[j], VK_NULL_HANDLE);
				}

				return nullptr;
			}
		}

		return new VulkanFramebuffer(vkFramebuffers, images, imageViews, width, height);
	}

	void VulkanGraphics::DestroyFramebuffer(Framebuffer* pFramebuffer)
	{
		VulkanFramebuffer* pVulkanFramebuffer = static_cast<VulkanFramebuffer*>(pFramebuffer);

		for (UInt32 i = 0; i < pVulkanFramebuffer->GetImageViews().Size(); i++)
		{
			if (pVulkanFramebuffer->GetImageViews()[i] != nullptr)
			{
				DestroyImageView(pVulkanFramebuffer->GetImageViews()[i]);
			}
		}

		for (UInt32 i = 0; i < pVulkanFramebuffer->GetImages().Size(); i++)
		{
			if (pVulkanFramebuffer->GetImages()[i] != nullptr)
			{
				DestroyImage(pVulkanFramebuffer->GetImages()[i]);
			}
		}

		for (UInt32 i = 0; i < pVulkanFramebuffer->GetVkFramebuffers().Size(); i++)
		{
			vkDestroyFramebuffer(mpDevice->GetDeviceHandle(), 
				pVulkanFramebuffer->GetVkFramebuffers()[i], VK_NULL_HANDLE);
		}

		delete pVulkanFramebuffer;
	}

	Shader* VulkanGraphics::CreateShader(const String& name, const Array<Byte>& binary)
	{
		VkShaderModule			vkShader;
		SpirvReflection			reflection;
		Array<SpirvUniform>		uniforms;
		Array<SpirvAttribute>	attributes;
		ShaderType				shaderType;
		VkShaderStageFlagBits	vkStageFlags;

		VkShaderModuleCreateInfo shaderInfo = {};
		shaderInfo.sType	= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderInfo.codeSize = binary.Size();
		shaderInfo.pCode	= reinterpret_cast<const uint32_t*>(binary.Data());

		if (vkCreateShaderModule(mpDevice->GetDeviceHandle(), &shaderInfo, nullptr, &vkShader) != VK_SUCCESS)
		{
			Log::Error("Failed to create shader: vkCreateShaderModule failed!");
			return nullptr;
		}

		if (!SpirvParseReflection(&reflection, binary))
		{
			Log::Error("Failed to create shader: Failed to parse SPIR-V reflection data!");
			vkDestroyShaderModule(mpDevice->GetDeviceHandle(), vkShader, VK_NULL_HANDLE);
			return nullptr;
		}

		SpirvExtractUniforms(uniforms, reflection);
		SpirvExtractAttributes(attributes, reflection);

		switch (reflection.executionModel)
		{
			case SpvExecutionModelVertex:
			{
				shaderType		= SHADER_VERTEX;
				vkStageFlags	= VK_SHADER_STAGE_VERTEX_BIT;

				break;
			}
			case SpvExecutionModelTessellationControl:
			{
				shaderType		= SHADER_TESSELLATION_CONTROL;
				vkStageFlags	= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;

				break;
			}
			case SpvExecutionModelTessellationEvaluation:
			{
				shaderType		= SHADER_TESSELLATION_EVALUATION;
				vkStageFlags	= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;

				break;
			}
			case SpvExecutionModelGeometry:
			{
				shaderType		= SHADER_GEOMETRY;
				vkStageFlags	= VK_SHADER_STAGE_GEOMETRY_BIT;

				break;
			}
			case SpvExecutionModelFragment:
			{
				shaderType		= SHADER_FRAGMENT;
				vkStageFlags	= VK_SHADER_STAGE_FRAGMENT_BIT;

				break;
			}
			case SpvExecutionModelGLCompute:
			{
				shaderType		= SHADER_COMPUTE;
				vkStageFlags	= VK_SHADER_STAGE_COMPUTE_BIT;

				break;
			}
			case SpvExecutionModelKernel:
			{
				shaderType		= SHADER_KERNEL;
				vkStageFlags	= VK_SHADER_STAGE_COMPUTE_BIT; // Correct?

				break;
			}
			case SpvExecutionModelTaskNV:
			{
				shaderType		= SHADER_TASK;
				vkStageFlags	= VK_SHADER_STAGE_TASK_BIT_NV;

				break;
			}
			case SpvExecutionModelMeshNV:
			{
				shaderType		= SHADER_MESH;
				vkStageFlags	= VK_SHADER_STAGE_MESH_BIT_NV;

				break;
			}
			case SpvExecutionModelRayGenerationKHR:
			{
				shaderType		= SHADER_RAY_GENERATION;
				vkStageFlags	= VK_SHADER_STAGE_RAYGEN_BIT_KHR;

				break;
			}
			case SpvExecutionModelIntersectionKHR:
			{
				shaderType		= SHADER_INTERSECTION;
				vkStageFlags	= VK_SHADER_STAGE_INTERSECTION_BIT_KHR;

				break;
			}
			case SpvExecutionModelAnyHitKHR:
			{
				shaderType		= SHADER_ANY_HIT;
				vkStageFlags	= VK_SHADER_STAGE_ANY_HIT_BIT_KHR;

				break;
			}
			case SpvExecutionModelClosestHitKHR:
			{
				shaderType		= SHADER_CLOSEST_HIT;
				vkStageFlags	= VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;

				break;
			}
			case SpvExecutionModelMissKHR:
			{
				shaderType		= SHADER_MISS;
				vkStageFlags	= VK_SHADER_STAGE_MISS_BIT_KHR;

				break;
			}
			case SpvExecutionModelCallableKHR:
			{
				shaderType		= SHADER_CALLABLE;
				vkStageFlags	= VK_SHADER_STAGE_CALLABLE_BIT_KHR;

				break;
			}
		}

		return new VulkanShader(name, shaderType, 
			vkShader, vkStageFlags, reflection.entryName, uniforms, attributes);
	}

	void VulkanGraphics::DestroyShader(Shader* pShader)
	{
		VulkanShader* pVulkanShader = static_cast<VulkanShader*>(pShader);
		vkDestroyShaderModule(mpDevice->GetDeviceHandle(), pVulkanShader->GetVkShader(), VK_NULL_HANDLE);
		delete pVulkanShader;
	}

	GraphicsPipeline* VulkanGraphics::CreateGraphicsPipeline(const GraphicsPipelineInfo& info, UInt32 subpass)
	{
		VkPipeline vkPipeline = VK_NULL_HANDLE;

		VkPrimitiveTopology vkTopology		= VulkanUtil::PrimitiveTopologyToVkPrimitiveTopology(info.topology);
		VkPolygonMode		vkPolygonMode	= VulkanUtil::PolygonModeToVkPolygonMode(info.polygonMode);
		VkCullModeFlags		vkCullMode		= VulkanUtil::CullModeToVkCullMode(info.cullMode);
		VkFrontFace			vkFrontFace		= VulkanUtil::FaceWindToVkFrontFace(info.faceWind);
		UInt32				multisamples	= VulkanUtil::MultisampleToVkMultisample(info.multisample);

		VkViewport vkViewport;
		vkViewport.x		= info.viewport.x;
		vkViewport.y		= info.viewport.height - info.viewport.y;
		vkViewport.width	= info.viewport.width;
		vkViewport.height	= -info.viewport.height;
		vkViewport.minDepth = info.viewport.minDepth;
		vkViewport.maxDepth = info.viewport.maxDepth;

		VkRect2D vkScissor;
		vkScissor.offset.x		= info.scissor.x;
		vkScissor.offset.y		= info.scissor.y;
		vkScissor.extent.width	= info.scissor.width;
		vkScissor.extent.height = info.scissor.height;

		Array<VkPipelineShaderStageCreateInfo>				shaderStageInfos;
		Array<Map<String, VkDescriptorSetLayoutBinding>>	descriptorSetBindings(16); // @Todo: querry max supported sets
		Array<UInt32>										descriptorSetSizes(16);
		Array<VkDescriptorSetLayout>						descriptorSetLayouts;
		
		USize maxSetIndex = 0;

		for (Shader* pShader : info.shaders)
		{
			VulkanShader* pVulkanShader = static_cast<VulkanShader*>(pShader);

			VkPipelineShaderStageCreateInfo vkShaderStageInfo = {};
			vkShaderStageInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vkShaderStageInfo.flags					= 0;
			vkShaderStageInfo.pSpecializationInfo	= nullptr;
			vkShaderStageInfo.stage					= pVulkanShader->GetVkShaderStage();
			vkShaderStageInfo.module				= pVulkanShader->GetVkShader();
			vkShaderStageInfo.pName					= pVulkanShader->GetEntryPoint().Str();
			vkShaderStageInfo.pNext					= nullptr;

			shaderStageInfos.PushBack(vkShaderStageInfo);

			const Array<SpirvUniform>& uniforms = pVulkanShader->GetUniforms();

			for (UInt32 i = 0; i < uniforms.Size(); i++)
			{
				const SpirvUniform& uniform = uniforms[i];

				if (descriptorSetBindings[uniform.set].Contains(uniform.name))
				{
					descriptorSetBindings[uniform.set][uniform.name].stageFlags |= pVulkanShader->GetVkShaderStage();
				}
				else
				{
					VkDescriptorSetLayoutBinding vkBinding;
					vkBinding.binding				= uniform.binding;
					vkBinding.descriptorCount		= 1;
					vkBinding.descriptorType		= uniform.descriptorType;
					vkBinding.stageFlags			= pVulkanShader->GetVkShaderStage();
					vkBinding.pImmutableSamplers	= nullptr;

					// @TODO: check set bounds < 16
					descriptorSetBindings[uniform.set].Put(uniform.name, vkBinding);
					descriptorSetSizes[uniform.set] += uniform.sizeBytes;
				}

				if (uniform.set > maxSetIndex)
				{
					maxSetIndex = uniform.set;
				}
			}

			// @Todo: samplers, etc.
		}

		descriptorSetLayouts.Resize(maxSetIndex + 1);

		UInt32 setIndex = 0;
		for (const Map<String, VkDescriptorSetLayoutBinding>& set : descriptorSetBindings)
		{
			if (setIndex > maxSetIndex)
			{
				break;
			}

			Array<VkDescriptorSetLayoutBinding> setBindings;

			for (const MapPair<String, VkDescriptorSetLayoutBinding>& binding : set)
			{
				setBindings.PushBack(binding.value);
			}

			VkDescriptorSetLayout vkDescriptorSetLayout;

			VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutInfo = {};
			vkDescriptorSetLayoutInfo.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			vkDescriptorSetLayoutInfo.flags			= 0;
			vkDescriptorSetLayoutInfo.bindingCount	= setBindings.Size();
			vkDescriptorSetLayoutInfo.pBindings		= setBindings.Data();

			if (vkCreateDescriptorSetLayout(mpDevice->GetDeviceHandle(), &vkDescriptorSetLayoutInfo, nullptr, &vkDescriptorSetLayout) != VK_SUCCESS)
			{
				Log::Error("Failed to create vulkan descriptor set layout: vkCreateDescriptorSetLayout failed!");
				// @TODO: Destroy other sets
				return nullptr;
			}

			descriptorSetLayouts[setIndex] = vkDescriptorSetLayout;

			setIndex++;
		}

		VkPipelineVertexInputStateCreateInfo vkVertexInputStateInfo = {};
		vkVertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vkVertexInputStateInfo.flags = 0;

		Array<VkVertexInputBindingDescription> bindingDescriptions;
		for (BufferAttachent& binding : info.bufferAttachments)
		{
			VkVertexInputBindingDescription vkBindingDescription;
			vkBindingDescription.binding	= binding.binding;
			vkBindingDescription.stride		= binding.stride;
			vkBindingDescription.inputRate	= VK_VERTEX_INPUT_RATE_VERTEX;

			bindingDescriptions.PushBack(vkBindingDescription);
		}

		vkVertexInputStateInfo.vertexBindingDescriptionCount = bindingDescriptions.Size();
		vkVertexInputStateInfo.pVertexBindingDescriptions = bindingDescriptions.Data();

		Array<VkVertexInputAttributeDescription> attributeDescriptions;
		UInt32 elementOffset = 0;

		for (VertexAttribute& attribute : info.vertexAttributes)
		{
			VkFormat vkFormat = VulkanUtil::AttributeTypeToVkFormat(attribute.type);

			VkVertexInputAttributeDescription vkAttributeDescription;
			vkAttributeDescription.binding	= attribute.binding;
			vkAttributeDescription.location = attribute.location;
			vkAttributeDescription.format	= vkFormat;
			vkAttributeDescription.offset	= elementOffset;

			attributeDescriptions.PushBack(vkAttributeDescription);

			elementOffset += VulkanUtil::AttributeTypeToSize(attribute.type);
		}

		vkVertexInputStateInfo.vertexAttributeDescriptionCount = attributeDescriptions.Size();
		vkVertexInputStateInfo.pVertexAttributeDescriptions = attributeDescriptions.Data();

		VkPipelineInputAssemblyStateCreateInfo vkInputAssemblyInfo = {};
		vkInputAssemblyInfo.sType		= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		vkInputAssemblyInfo.flags		= 0;
		vkInputAssemblyInfo.topology	= vkTopology;

		VkPipelineViewportStateCreateInfo vkViewportInfo = {};
		vkViewportInfo.sType			= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		vkViewportInfo.flags			= 0;
		vkViewportInfo.viewportCount	= 1;
		vkViewportInfo.pViewports		= &vkViewport;
		vkViewportInfo.scissorCount		= 1;
		vkViewportInfo.pScissors		= &vkScissor;

		VkPipelineRasterizationStateCreateInfo vkRasterizationInfo = {};
		vkRasterizationInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		vkRasterizationInfo.flags					= 0;
		vkRasterizationInfo.depthClampEnable		= VK_FALSE;
		vkRasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		vkRasterizationInfo.polygonMode				= vkPolygonMode;
		vkRasterizationInfo.cullMode				= vkCullMode;
		vkRasterizationInfo.frontFace				= vkFrontFace;
		vkRasterizationInfo.depthBiasEnable			= 0;
		vkRasterizationInfo.depthBiasConstantFactor = 0;
		vkRasterizationInfo.depthBiasClamp			= 0;
		vkRasterizationInfo.depthBiasSlopeFactor	= 0;
		vkRasterizationInfo.lineWidth				= info.lineWidth;

		UInt32 maxMultisamples = mpDevice->GetPhysicalDevice().GetPhysicalDeviceProperties().limits.framebufferColorSampleCounts;

		if (multisamples == 0)
		{
			multisamples = 1;
			Log::Warning("Invalid zero multisample value in pipeline creation, using multisamples value of 1");
		}

		if (multisamples > maxMultisamples)
		{
			multisamples = maxMultisamples;
			Log::Warning("Invalid maximum multisample value in pipeline creation, using max multisample value of %d", maxMultisamples);
		}

		VkPipelineMultisampleStateCreateInfo vkMultisampleInfo = {};
		vkMultisampleInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		vkMultisampleInfo.flags					= 0;
		vkMultisampleInfo.rasterizationSamples	= (VkSampleCountFlagBits)multisamples;
		vkMultisampleInfo.sampleShadingEnable	= VK_FALSE;
		vkMultisampleInfo.minSampleShading		= 0.0f;
		vkMultisampleInfo.pSampleMask			= nullptr;
		vkMultisampleInfo.alphaToCoverageEnable = VK_FALSE;
		vkMultisampleInfo.alphaToOneEnable		= VK_FALSE;

		VkPipelineDepthStencilStateCreateInfo vkDepthStencilInfo = {};
		vkDepthStencilInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		vkDepthStencilInfo.flags					= 0;
		vkDepthStencilInfo.depthTestEnable			= info.depthTesting;
		vkDepthStencilInfo.depthWriteEnable			= info.depthTesting ? VK_TRUE : VK_FALSE;
		vkDepthStencilInfo.depthCompareOp			= VK_COMPARE_OP_LESS;
		vkDepthStencilInfo.depthBoundsTestEnable	= VK_FALSE;
		vkDepthStencilInfo.stencilTestEnable		= VK_FALSE;
		vkDepthStencilInfo.front.failOp				= VK_STENCIL_OP_KEEP;
		vkDepthStencilInfo.front.passOp				= VK_STENCIL_OP_KEEP;
		vkDepthStencilInfo.front.depthFailOp		= VK_STENCIL_OP_KEEP;
		vkDepthStencilInfo.front.compareOp			= VK_COMPARE_OP_LESS;
		vkDepthStencilInfo.front.compareMask		= 0;
		vkDepthStencilInfo.front.writeMask			= 0;
		vkDepthStencilInfo.front.reference			= 0;
		vkDepthStencilInfo.back.failOp				= VK_STENCIL_OP_KEEP;
		vkDepthStencilInfo.back.passOp				= VK_STENCIL_OP_KEEP;
		vkDepthStencilInfo.back.depthFailOp			= VK_STENCIL_OP_KEEP;
		vkDepthStencilInfo.back.compareOp			= VK_COMPARE_OP_LESS;
		vkDepthStencilInfo.back.compareMask			= 0;
		vkDepthStencilInfo.back.writeMask			= 0;
		vkDepthStencilInfo.back.reference			= 0;
		vkDepthStencilInfo.minDepthBounds			= 0.0f;
		vkDepthStencilInfo.maxDepthBounds			= 1.0f;

		VkPipelineColorBlendStateCreateInfo vkColorBlendInfo = {};
		vkColorBlendInfo.sType			= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		vkColorBlendInfo.flags			= 0;
		vkColorBlendInfo.logicOpEnable	= VK_FALSE;
		vkColorBlendInfo.logicOp		= VK_LOGIC_OP_COPY;

		Array<VkPipelineColorBlendAttachmentState> blendAttachments;
		for (BlendAttachment& blendAttachment : info.blendAttachments)
		{
			VkPipelineColorBlendAttachmentState vkBlendAttachmentState = {};
			vkBlendAttachmentState.blendEnable			= VK_TRUE;
			vkBlendAttachmentState.srcColorBlendFactor	= VK_BLEND_FACTOR_SRC_ALPHA;
			vkBlendAttachmentState.dstColorBlendFactor	= VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			vkBlendAttachmentState.colorBlendOp			= VK_BLEND_OP_ADD;
			vkBlendAttachmentState.srcAlphaBlendFactor	= VK_BLEND_FACTOR_ONE;
			vkBlendAttachmentState.dstAlphaBlendFactor	= VK_BLEND_FACTOR_ZERO;
			vkBlendAttachmentState.alphaBlendOp			= VK_BLEND_OP_ADD;
			vkBlendAttachmentState.colorWriteMask		= VK_COLOR_COMPONENT_R_BIT | 
															VK_COLOR_COMPONENT_G_BIT | 
															VK_COLOR_COMPONENT_B_BIT | 
															VK_COLOR_COMPONENT_A_BIT;

			blendAttachments.PushBack(vkBlendAttachmentState);
		}

		vkColorBlendInfo.attachmentCount	= blendAttachments.Size();
		vkColorBlendInfo.pAttachments		= blendAttachments.Data();
		vkColorBlendInfo.blendConstants[0]	= 1.0f;
		vkColorBlendInfo.blendConstants[1]	= 1.0f;
		vkColorBlendInfo.blendConstants[2]	= 1.0f;
		vkColorBlendInfo.blendConstants[3]	= 1.0f;

		VkPipelineLayout vkPipelineLayout;

		VkPipelineLayoutCreateInfo vkLayoutInfo = {};
		vkLayoutInfo.sType			= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		vkLayoutInfo.flags			= 0;
		vkLayoutInfo.setLayoutCount = descriptorSetLayouts.Size();
		vkLayoutInfo.pSetLayouts	= descriptorSetLayouts.Data();

		// TODO: Support push constants?
		vkLayoutInfo.pushConstantRangeCount = 0;
		vkLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(mpDevice->GetDeviceHandle(), &vkLayoutInfo, nullptr, &vkPipelineLayout) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan pipeline layout: vkCreatePipelineLayout failed!");
			// @TODO: Destroy all descriptorSets
			return nullptr;
		}

		///////////////////////////////////////////////////////////////

		VulkanRenderpass* pVulkanRenderpass = static_cast<VulkanRenderpass*>(info.pRenderpass);

		VkGraphicsPipelineCreateInfo vkPipelineInfo = {};
		vkPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		Bool8 parent = false;
		if (parent)
		{
			vkPipelineInfo.flags				|= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
			vkPipelineInfo.basePipelineHandle	= 0;//parentId;
			vkPipelineInfo.basePipelineIndex	= 0;
		}

		vkPipelineInfo.stageCount			= shaderStageInfos.Size();
		vkPipelineInfo.pStages				= shaderStageInfos.Data();
		vkPipelineInfo.pVertexInputState	= &vkVertexInputStateInfo;
		vkPipelineInfo.pInputAssemblyState	= &vkInputAssemblyInfo;
		vkPipelineInfo.pTessellationState	= nullptr;
		vkPipelineInfo.pViewportState		= &vkViewportInfo;
		vkPipelineInfo.pRasterizationState	= &vkRasterizationInfo;
		vkPipelineInfo.pMultisampleState	= &vkMultisampleInfo;
		vkPipelineInfo.pDepthStencilState	= &vkDepthStencilInfo;
		vkPipelineInfo.pColorBlendState		= &vkColorBlendInfo;
		vkPipelineInfo.pDynamicState		= nullptr;
		vkPipelineInfo.layout				= vkPipelineLayout;
		vkPipelineInfo.renderPass			= pVulkanRenderpass->GetVkRenderPass();
		vkPipelineInfo.subpass				= subpass;

		if (vkCreateGraphicsPipelines(mpDevice->GetDeviceHandle(), VK_NULL_HANDLE, 1, &vkPipelineInfo, nullptr, &vkPipeline) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan graphics pipeline: vkCreateGraphicsPipelines failed!");
			// @TODO: Destroy everything
			return nullptr;
		}

		Array<VulkanDescriptorSetInfo> descriptorSetInfos(descriptorSetLayouts.Size());

		for (UInt32 i = 0; i < descriptorSetInfos.Size(); i++)
		{
			VulkanDescriptorSetInfo& descriptorSetInfo = descriptorSetInfos[i];

			descriptorSetInfo.vkDescriptorSetLayout = descriptorSetLayouts[i];
			descriptorSetInfo.setSizeBytes			= descriptorSetSizes[i];

			for (const MapPair<String, VkDescriptorSetLayoutBinding>& binding : descriptorSetBindings[i])
			{
				descriptorSetInfo.bindings.PushBack(binding.value);
			}
		}

		VulkanGraphicsPipeline* pPipeline = new VulkanGraphicsPipeline(vkPipeline, mpDevice, descriptorSetInfos, vkPipelineInfo, info);

		pPipeline->SetupUniformStates(3); //TEMP

		return pPipeline;
	}

	void VulkanGraphics::DestroyGraphicsPipeline(GraphicsPipeline* pPipeline)
	{

	}

	CommandBuffer* VulkanGraphics::CreateCommandBuffer(CommandBufferType type)
	{
		return new VulkanCommandBuffer(mpDevice, type);
	}

	void VulkanGraphics::DestroyCommandBuffer(CommandBuffer* pCommandBuffer)
	{
		VulkanCommandBuffer* pVulkanComandBuffer = static_cast<VulkanCommandBuffer*>(pCommandBuffer);

		vkFreeCommandBuffers(mpDevice->GetDeviceHandle(),
			pVulkanComandBuffer->GetVkCommandPool(),
			pVulkanComandBuffer->GetCommandBufferCount(),
			pVulkanComandBuffer->GetCommandbuffers().Data());

		delete pVulkanComandBuffer;
	}

	void VulkanGraphics::CopyBuffer(Buffer* pSource, Buffer* pDest)
	{
		VulkanBuffer* pVulkanBufferSource	= static_cast<VulkanBuffer*>(pSource);
		VulkanBuffer* pVulkanBufferDest		= static_cast<VulkanBuffer*>(pDest);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool			= mpDevice->GetTransferCommandPoolHandle();
		allocInfo.commandBufferCount	= 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(mpDevice->GetDeviceHandle(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferCopy copyRegion{};
		copyRegion.srcOffset	= 0;
		copyRegion.dstOffset	= 0;
		copyRegion.size			= pVulkanBufferSource->GetSize();

		vkCmdCopyBuffer(commandBuffer, pVulkanBufferSource->GetVkBuffer(), pVulkanBufferDest->GetVkBuffer(), 1, &copyRegion);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount	= 1;
		submitInfo.pCommandBuffers		= &commandBuffer;

		vkQueueSubmit(mpDevice->GetTransferQueue().GetQueueHandle(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(mpDevice->GetTransferQueue().GetQueueHandle());

		vkFreeCommandBuffers(mpDevice->GetDeviceHandle(), mpDevice->GetTransferCommandPoolHandle(), 1, &commandBuffer);
	}

	void VulkanGraphics::BindUniformBuffer(Pipeline* pPipeline, UInt32 set, UInt32 buffer, Buffer* pBuffer)
	{
		/*
		VulkanGraphicsPipeline* pGraphicsPipeline = static_cast<VulkanGraphicsPipeline*>(pipeline);
		VulkanBuffer* pVulkanBuffer = static_cast<VulkanBuffer*>(buffer);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer	= pVulkanBuffer->vkBuffer;
		bufferInfo.offset	= 0;
		bufferInfo.range	= pVulkanBuffer->sizeBytes;

		VkWriteDescriptorSet descriptorWrite {};
		descriptorWrite.sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet			= pGraphicsPipeline->descriptorSets[bufferIndex];
		descriptorWrite.dstBinding		= binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo		= &bufferInfo;

		vkUpdateDescriptorSets(mpDevice->GetDeviceHandle(), 1, &descriptorWrite, 0, nullptr);
		*/
	}

	void VulkanGraphics::Submit(Context* pContext, CommandBuffer* pCommandBuffer)
	{
		VulkanCommandBuffer*	pVulkanCommandBuffer	= static_cast<VulkanCommandBuffer*>(pCommandBuffer);
		VulkanContext*			pVulkanContext			= static_cast<VulkanContext*>(pContext);
		VulkanSwapchain*		pVulkanSwapchain		= pVulkanContext->GetSwapchain();

		VulkanQueue& graphicsQueue = mpDevice->GetGraphicsQueue(); // @TODO: TEMP!!!

		if (!pVulkanCommandBuffer->IsBuilt() || 
			pVulkanCommandBuffer->GetCommandBufferCount() != pVulkanSwapchain->GetBackbufferCount())
		{
			vkQueueWaitIdle(graphicsQueue.GetQueueHandle());

			pVulkanCommandBuffer->BuildBuffers(pVulkanSwapchain->GetBackbufferCount());

			if (!pVulkanCommandBuffer->IsDynamic())
			{
				pVulkanCommandBuffer->RecordStatic();
			}
		}

		UInt32 frameIndex = pVulkanSwapchain->GetFrameIndex();

		if (pVulkanCommandBuffer->IsDynamic())
		{
			pVulkanCommandBuffer->RecordDynamic(frameIndex);
		}

		VkCommandBuffer commandBuffers[]	= { pVulkanCommandBuffer->GetCommandbuffers()[frameIndex] };
		VkPipelineStageFlags waitStages[]	= { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore waitSemaphores[]		= { pVulkanSwapchain->GetCurrentAcquiredSemaphore() };
		VkSemaphore signalSemaphores[]		= { pVulkanSwapchain->GetCurrentCompleteSemaphore() };

		//vkWaitForFences(mpDevice->GetDeviceHandle(), 1, &pVulkanSwapchain->allFences[pVulkanSwapchain->frameIndex], VK_TRUE, UINT64_MAX);
		//vkResetFences(mpDevice->GetDeviceHandle(), 1, &pVulkanSwapchain->allFences[pVulkanSwapchain->frameIndex]);

		VkSubmitInfo submitInfo{};
		submitInfo.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount	= 1;
		submitInfo.pWaitSemaphores		= waitSemaphores;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores	= signalSemaphores;
		submitInfo.pWaitDstStageMask	= waitStages;
		submitInfo.commandBufferCount	= 1;
		submitInfo.pCommandBuffers		= commandBuffers;

		if (vkQueueSubmit(graphicsQueue.GetQueueHandle(), 1, &submitInfo, pVulkanSwapchain->GetCurrentFence()) != VK_SUCCESS)
		{
			Log::Critical("Failed to submit queue: vkQueueSubmit failed!");
		}
	}

	Bool8 VulkanGraphics::Init()
	{
		/* Creating Instance */

		Log::Info("Initializing Vulkan Instance.");

		/* Validation Layers */
		Array<const char*> preferedValidationLayers;
		preferedValidationLayers.PushBack("VK_LAYER_KHRONOS_validation");

		/* Extentions */
		Array<const char*> preferedInstanceExtentions;
		preferedInstanceExtentions.PushBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		preferedInstanceExtentions.PushBack(VK_KHR_SURFACE_EXTENSION_NAME);
		preferedInstanceExtentions.PushBack("VK_KHR_win32_surface");

		if (VulkanUtil::EnumerateVkLayerProperties(mAvailableLayers) != VK_SUCCESS)
		{
			Log::Warning("Failed to create a vulkan instance: Unable to enumerate validation layers!");
			return false;
		}

		for (const char* layerName : preferedValidationLayers)
		{
			for (const VkLayerProperties& layerProp : mAvailableLayers)
			{
				if (strcmp(layerName, layerProp.layerName) == 0)
				{
					mEnabledValidationLayerNames.PushBack(layerName);
					goto layerFound;
				}
			}

			Log::Warning("Attempted to enable unsupported vulkan instance layer [\'%s\']!", layerName);

			layerFound:;
		}

		if (VulkanUtil::EnumerateVkExtensionProperties(mAvailableExtensions) != VK_SUCCESS)
		{
			Log::Warning("Failed to create a vulkan instance: Unable to enumerate extensions!");
			return false;
		}

		for (const char* extName : preferedInstanceExtentions)
		{
			for (const VkExtensionProperties& extProp : mAvailableExtensions)
			{
				if (strcmp(extName, extProp.extensionName) == 0)
				{
					mEnabledExtensionNames.PushBack(extName);
					goto extFound;
				}
			}

			Log::Warning("Attempted to enable unsupported vulkan instance extension [\'%s\']!", extName);

			extFound:;
		}

		String gameName = StringWToStringA(Engine::GetInstance()->GetGameInfo().name);

		if (VulkanUtil::CreateVkInstance(&mvkInstance, gameName.Str(),
			mEnabledValidationLayerNames, mEnabledExtensionNames) != VK_SUCCESS)
		{
			Log::Critical("Unable to create Vulkan Instance.");
			return false;
		}

		/* Create Device */

		Log::Info("Initializing Vulkan Devices.");

		Array<String> preferedDeviceExtensions;
		preferedDeviceExtensions.PushBack(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		if (VulkanUtil::EnumerateVkPhysicalDevices(mvkInstance, mAvailablePhysicalDevices) != VK_SUCCESS)
		{
			Log::Error("Failed to initialize vulkan graphics device: vkEnumeratePhysicalDevices failed!");
			return false;
		}

		Log::Info("%d devices found:", mAvailablePhysicalDevices.Size());

		mPhysicalDevices.Resize(mAvailablePhysicalDevices.Size());

		Int32 maxDeviceScore = -1;
		VulkanPhysicalDevice* pPhyiscalDeviceCandidate = nullptr;

		Int32 deviceIndex = 0;
		for (const VkPhysicalDevice physicalDevice : mAvailablePhysicalDevices)
		{
			mPhysicalDevices[deviceIndex] = VulkanPhysicalDevice(physicalDevice);

			Int32 deviceScore = mPhysicalDevices[deviceIndex].GetDeviceScore();

			if (deviceScore > maxDeviceScore)
			{
				maxDeviceScore = deviceScore;
				pPhyiscalDeviceCandidate = &mPhysicalDevices[deviceIndex];
			}

			Log::Info(" - %s", mPhysicalDevices[deviceIndex].GetDeviceName().Str());

			++deviceIndex;
		}

		if (pPhyiscalDeviceCandidate == nullptr)
		{
			Log::Critical("Failed to initialize vulkan graphics device: No suitable graphics adapter was detected!");
			return false;
		}

		mpDevice = new VulkanDevice(pPhyiscalDeviceCandidate, preferedDeviceExtensions);

		Log::Info("Using Best Device:");
		Log::Info("--------------------------");
		Log::Info("Device Name:   %s", mpDevice->GetPhysicalDevice().GetDeviceName().Str());
		Log::Info("Device Vendor: %s", VulkanUtil::VendorNameFromID(mpDevice->GetPhysicalDevice().GetPhysicalDeviceProperties().vendorID).Str());
		Log::Info("Device Memory: %dMB", mpDevice->GetPhysicalDevice().GetPhysicalDeviceMemoryProperties().memoryHeaps[0].size / (1024 * 1024));
		Log::Info("--------------------------");

		if (!mpDevice->IsValidDevice())
		{
			Log::Critical("Failed to initialize vulkan graphics device: No valid device was created!");
			return false;
		}

		return true;
	}

	void VulkanGraphics::Update(Float32 delta)
	{
		for (Context* pContext : mViewports)
		{
			VulkanContext* pVulkanContext = static_cast<VulkanContext*>(pContext);
			VulkanSwapchain* pVulkanSwapchain = pVulkanContext->GetSwapchain();
			
			pVulkanSwapchain->AdvanceFrame();
			pContext->GetRenderer()->Render(pContext, nullptr);
			pVulkanSwapchain->Present();
		}
	}
}