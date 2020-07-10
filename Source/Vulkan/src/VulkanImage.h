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
		VkImage							mVulkanImage;
		VkFormat						mVulkanFormat;
		VkImageUsageFlags				mVulkanUsage;
		VkImageType						mVulkanImageType;
		VulkanDeviceMemoryAllocation*	mpMemory;
		Bool8							mValidImage;

	public:
		VulkanImage() = default;

		VulkanImage(
			VulkanDevice& device,
			VkImageType imageType, VkImageUsageFlags usage, VkFormat format,
			UInt32 width, UInt32 height, UInt32 depth,
			UInt32 mipLevels, UInt32 layers);

		VulkanImage(
			VulkanDevice& device,
			VkImage sourceImage,
			VkImageType imageType, VkImageUsageFlags usage, VkFormat format,
			UInt32 width, UInt32 height, UInt32 depth, 
			UInt32 mipLevels, UInt32 layers);

		VulkanDevice& GetParentDevice() { return *mpParentDevice; }

		VkImage GetImageHandle() { return mVulkanImage; }
		VkFormat GetVulkanFormat() const { return mVulkanFormat; }
		VkImageUsageFlags GetVulkanUsageFlags() const { return mVulkanUsage; }
		VkImageType GetVulkanImageType() const { return mVulkanImageType; }
		VulkanDeviceMemoryAllocation* GetMemoryAllocation() { return mpMemory; }

		Bool8 IsExternal() const { return mpMemory == nullptr; }
		Bool8 IsValidImage() const { return mValidImage; }
	};

	class QUARTZ_API VulkanImageView : public GFXImageView
	{
	private:
		VkImageView		mImageView;
		Bool8			mIsValidImageView;

	public:
		VulkanImageView() = default;

		VulkanImageView(VulkanImage& image, VkImageViewType viewType,
			VkImageAspectFlags aspects, UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layerLevels);

		VkImageView GetImageViewHandle() const { return mImageView; }

		Bool8 IsValidImageView() const { return mIsValidImageView; }
	};
}