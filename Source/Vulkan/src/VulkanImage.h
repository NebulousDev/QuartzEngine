#pragma once

#include "graphics/Image.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanImage : public Image
	{
	private:
		VkImage				mvkImage;
		VkDeviceMemory		mvkMemory;
		VkImageType			mvkImageType;
		VkFormat			mvkFormat;
		VkImageUsageFlags	mvkUsageFlags;

	public:
		VulkanImage(VkImage vkImage, VkDeviceMemory vkMemory, VkFormat vkFormat,
			VkImageUsageFlags vkUsage, VkImageType vkImageType,
			ImageType type, UInt32 width, UInt32 height, UInt32 depth,
			UInt32 layers, UInt32 mips, ImageFormat format, ImageUsages usages);

		FORCE_INLINE VkImage			GetVkImage() { return mvkImage; }
		FORCE_INLINE VkImageType		GetVkImageType() { return mvkImageType; }
		FORCE_INLINE VkFormat			GetVkFormat() { return mvkFormat; }
		FORCE_INLINE VkImageUsageFlags	GetVkImageUsageFlags() { return mvkUsageFlags; }
		FORCE_INLINE VkDeviceMemory		GetVkDeviceMemory() { return mvkMemory; }
	};

	class QUARTZ_API VulkanImageView : public ImageView
	{
	private:
		VkImageView			mvkImageView;
		VkImageViewType		mvkViewType;
		VkImageAspectFlags	mvkAspectFlags;

	public:
		VulkanImageView(VkImageView vkImageView, VkImageViewType vkViewType,
			VkImageAspectFlags vkAspectFlags, Image* pImage, ImageViewType type,
			UInt32 width, UInt32 height, UInt32 depth, UInt32 layers, UInt32 layerStart,
			UInt32 mips, UInt32 mipStart, ImageFormat format, ImageViewUsage usage);

		FORCE_INLINE VkImageView		GetVkImageView() { return mvkImageView; }
		FORCE_INLINE VkImageViewType	GetVkImageViewType() { return mvkViewType; }
		FORCE_INLINE VkImageAspectFlags GetVkImageAspectFlags() { return mvkAspectFlags; }
	};
}