#include "VulkanImage.h"

#include "log/Log.h"

namespace Quartz
{
	VulkanImage::VulkanImage(VulkanDevice& device, VkImageType imageType, VkFormat format, UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, UInt32 layers)
	{
		VkImageCreateInfo imageInfo = {};
		imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageInfo.imageType = imageType;
		imageInfo.format = format;
		imageInfo.extent.width = width;
		imageInfo.extent.height = height;
		imageInfo.extent.depth = depth;
		imageInfo.mipLevels = mipLevels;
		imageInfo.arrayLayers = layers;
		imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		if (vkCreateImage(device.GetDeviceHandle(), &imageInfo, nullptr, &mImage) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan image: vkCreateImage failed!");
			return;
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device.GetDeviceHandle(), mImage, &memRequirements);

		VulkanDeviceMemoryAllocator& allocator = device.GetDeviceMemoryAllocator();
		mpMemory = allocator.Allocate(memRequirements.size, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (mpMemory == nullptr)
		{
			Log.Error("Failed to create vulkan image: Failed to allocate device memory!");
			return;
		}

		if (vkBindImageMemory(device.GetDeviceHandle(), mImage, mpMemory->GetDeviceMemoryHandle(), 0) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan buffer object: vkBindBufferMemory failed!");

			allocator.Free(mpMemory);
			vkDestroyImage(device.GetDeviceHandle(), mImage, nullptr);

			return;
		}

		mFormat = format;

		mValidImage = true;
	}

	VulkanImageView::VulkanImageView(const VulkanDevice& device, VkImage image, VkImageViewType viewType,
		VkFormat format, VkImageAspectFlags aspects, UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layerLevels)
	{
		VkImageViewCreateInfo imageViewInfo = {};

		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.flags = 0;
		imageViewInfo.image = image;
		imageViewInfo.viewType = viewType;
		imageViewInfo.format = format;

		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewInfo.subresourceRange.aspectMask = aspects;
		imageViewInfo.subresourceRange.baseMipLevel = mipStart;
		imageViewInfo.subresourceRange.levelCount = mipLevels;
		imageViewInfo.subresourceRange.baseArrayLayer = layerStart;
		imageViewInfo.subresourceRange.layerCount = layerLevels;

		if (vkCreateImageView(device.GetDeviceHandle(), &imageViewInfo, nullptr, &mImageView) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan image view: vkCreateImageView failed!");
			return;
		}

		mFormat = format;

		mIsValidImageView = true;
	}
}

