#pragma once

#include "Common.h"
#include "graphics\GFXImage.h"
#include "VulkanDevice.h"
#include "VulkanDeviceMemory.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanImage : public GFXImage
	{
	private:
		VulkanDevice*					mpParentDevice;
		VkImage							mImage;
		VkFormat						mFormat;
		VulkanDeviceMemoryAllocation*	mpMemory;
		Bool8							mValidImage;

	public:
		VulkanImage() = default;
		VulkanImage(
			VulkanDevice& device,
			VkImageType imageType, VkFormat format,
			UInt32 width, UInt32 height, UInt32 depth, 
			UInt32 mipLevels, UInt32 layers);

		VulkanDevice& GetParentDevice() { return *mpParentDevice; }

		VkImage GetImageHandle() { return mImage; }
		VkFormat GetFormat() { return mFormat; }
		VulkanDeviceMemoryAllocation& GetMemoryAllocation() { return *mpMemory; }

		Bool8 IsValidImage() const { return mValidImage; }
	};

	class QUARTZ_API VulkanImageView : public GFXImageView
	{
	private:
		VkImageView mImageView;
		VkFormat	mFormat;
		Bool8		mIsValidImageView;

	public:
		VulkanImageView() = default;

		VulkanImageView(const VulkanDevice& device, VkImage image, VkImageViewType viewType,
			VkFormat format, VkImageAspectFlags aspects, UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layerLevels);

		VkImageView GetImageViewHandle() const { return mImageView; }
		VkFormat GetFormat() { return mFormat; }

		Bool8 IsValidImageView() const { return mIsValidImageView; }
	};
}