#include "VulkanImage.h"

namespace Quartz
{
	VulkanImage::VulkanImage(VkImage vkImage, VkDeviceMemory vkMemory, VkFormat vkFormat, VkImageUsageFlags vkUsage,
		VkImageType vkImageType, ImageType type, UInt32 width, UInt32 height, UInt32 depth, 
		UInt32 layers, UInt32 mips, ImageFormat format, ImageUsages usages)
		: mvkImage(vkImage),
		mvkMemory(vkMemory),
		mvkFormat(vkFormat),
		mvkUsageFlags(vkUsage),
		mvkImageType(vkImageType),
		Image(type, width, height, depth, layers, mips, format, usages)
	{
		// Nothing
	}

	VulkanImageView::VulkanImageView(VkImageView vkImageView, VkImageViewType vkViewType,
		VkImageAspectFlags vkAspectFlags, Image* pImage, ImageViewType type, 
		UInt32 width, UInt32 height, UInt32 depth, UInt32 layers, UInt32 layerStart, 
		UInt32 mips, UInt32 mipStart, ImageFormat format, ImageViewUsage usage)
		: mvkImageView(vkImageView),
		mvkViewType(vkViewType),
		mvkAspectFlags(vkAspectFlags),
		ImageView(pImage, type, width, height, depth, 
			layers, layerStart, mips, mipStart, format, usage)
	{
		// Nothing
	}
}

