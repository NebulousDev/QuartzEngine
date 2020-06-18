#include "VulkanImageView.h"

#include "log/log.h"

namespace Quartz
{
	VulkanImageView::VulkanImageView(const VulkanDevice& device, VkImage image, VkImageViewType viewType,
		VkFormat format, VkImageAspectFlags aspects, UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layerLevels)
	{
		VkImageViewCreateInfo imageViewInfo = {};

		imageViewInfo.sType		= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.flags		= 0;
		imageViewInfo.image		= image;
		imageViewInfo.viewType	= viewType;
		imageViewInfo.format	= format;

		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewInfo.subresourceRange.aspectMask		= aspects;
		imageViewInfo.subresourceRange.baseMipLevel		= aspects;
		imageViewInfo.subresourceRange.levelCount		= aspects;
		imageViewInfo.subresourceRange.baseArrayLayer	= aspects;
		imageViewInfo.subresourceRange.layerCount		= aspects;

		if (vkCreateImageView(device.GetDeviceHandle(), &imageViewInfo, nullptr, &mImageView) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan image view: vkCreateImageView failed!");
			return;
		}

		mIsValidImageView = true;
	}
}

