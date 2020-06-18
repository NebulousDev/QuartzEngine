#pragma once

#include "Common.h"

#include "VulkanDevice.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanImageView
	{
	private:
		VkImageView mImageView;
		Bool8		mIsValidImageView;

	public:
		VulkanImageView() = default;

		VulkanImageView(const VulkanDevice& device, VkImage image, VkImageViewType viewType,
			VkFormat format, VkImageAspectFlags aspects, UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layerLevels);
	
		Bool8 IsValidImageView() const { return mIsValidImageView; }
	};
}