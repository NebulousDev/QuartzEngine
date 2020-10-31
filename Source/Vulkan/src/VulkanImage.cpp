#include "VulkanImage.h"

#include "log/Log.h"

namespace Quartz
{
	GFXFormat FormatFromVkFormat(VkFormat format)
	{
		switch (format)
		{
			case VK_FORMAT_R8_UNORM:			return FORMAT_R8_UNORM;
			case VK_FORMAT_R8G8_UNORM:			return FORMAT_RG8_UNORM;
			case VK_FORMAT_R8G8B8_UNORM:		return FORMAT_RGB8_UNORM;
			case VK_FORMAT_R8G8B8A8_UNORM:		return FORMAT_RGBA8_UNORM;
			case VK_FORMAT_B8G8R8A8_UNORM:		return FORMAT_BGRA8_UNORM;
			case VK_FORMAT_D24_UNORM_S8_UINT:	return FORMAT_DEPTH24_UNORM_STENCIL8_UINT;
			default:							return FORMAT_UNDEFINED;
		}
	}

	GFXImageType ImageTypeFromVkImageType(VkImageType type)
	{
		switch (type)
		{
			case VK_IMAGE_TYPE_1D:	return IMAGE_TYPE_1D;
			case VK_IMAGE_TYPE_2D:	return IMAGE_TYPE_2D;
			case VK_IMAGE_TYPE_3D:	return IMAGE_TYPE_3D;
			default:				return IMAGE_TYPE_2D;
		}
	}

	VulkanImage::VulkanImage(VulkanDevice& device, VkImage sourceImage, VkImageType imageType, VkImageUsageFlags usage, VkFormat format,
		UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, UInt32 layers)
	{
		mpParentDevice = &device;
		mVulkanImage = sourceImage;
		mVulkanFormat = format;
		mVulkanUsage = usage;
		mVulkanImageType = imageType;
		mpMemory = nullptr;

		mWidth = width;
		mHeight = height;
		mDepth = depth;
		mMipLevels = mipLevels;
		mLayers = layers;

		mImageFormat = FormatFromVkFormat(format);
		mImageType = ImageTypeFromVkImageType(imageType);

		mValidImage = true;
	}

	VulkanImage::VulkanImage(VulkanDevice& device, VkImageType imageType, VkImageUsageFlags usage, VkFormat format,
		UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, UInt32 layers)
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
		imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | usage; //VK_IMAGE_USAGE_SAMPLED_BIT;
		imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		if (vkCreateImage(device.GetDeviceHandle(), &imageInfo, nullptr, &mVulkanImage) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan image: vkCreateImage failed!");
			return;
		}

		VkMemoryRequirements memRequirements;
		vkGetImageMemoryRequirements(device.GetDeviceHandle(), mVulkanImage, &memRequirements);

		VulkanDeviceMemoryAllocator& allocator = device.GetDeviceMemoryAllocator();
		mpMemory = allocator.Allocate(memRequirements.size, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (mpMemory == nullptr)
		{
			Log.Error("Failed to create vulkan image: Failed to allocate device memory!");
			return;
		}

		if (vkBindImageMemory(device.GetDeviceHandle(), mVulkanImage, mpMemory->GetDeviceMemoryHandle(), 0) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan buffer object: vkBindBufferMemory failed!");

			allocator.Free(mpMemory);
			vkDestroyImage(device.GetDeviceHandle(), mVulkanImage, nullptr);

			return;
		}

		mpParentDevice = &device;
		mVulkanFormat = format;
		mVulkanUsage = usage;
		mVulkanImageType = imageType;

		mWidth = width;
		mHeight = height;
		mDepth = depth;
		mMipLevels = mipLevels;
		mLayers = layers;

		mImageFormat = FormatFromVkFormat(format);
		mImageType = ImageTypeFromVkImageType(imageType);

		mSizeBytes = memRequirements.size;
	}

	VulkanImageView::VulkanImageView(VulkanImage& image, VkImageViewType viewType, 
		VkImageAspectFlags aspects, UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layerLevels)
	{
		VkImageViewCreateInfo imageViewInfo = {};

		imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.flags = 0;
		imageViewInfo.image = image.GetImageHandle();
		imageViewInfo.viewType = viewType;
		imageViewInfo.format = image.GetVulkanFormat();

		imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		imageViewInfo.subresourceRange.aspectMask = aspects;
		imageViewInfo.subresourceRange.baseMipLevel = mipStart;
		imageViewInfo.subresourceRange.levelCount = mipLevels;
		imageViewInfo.subresourceRange.baseArrayLayer = layerStart;
		imageViewInfo.subresourceRange.layerCount = layerLevels;

		if (vkCreateImageView(image.GetParentDevice().GetDeviceHandle(), &imageViewInfo, nullptr, &mImageView) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan image view: vkCreateImageView failed!");
			return;
		}

		mpImage = &image;
		mMipStart = mipStart;
		mMipCount = mipLevels;
		mLayerStart = layerStart;
		mLayerCount = layerLevels;

		mIsValidImageView = true;
	}
}

