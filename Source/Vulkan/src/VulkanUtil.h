#pragma once

#include "util/Array.h"
#include "util/String.h"

#include "VulkanDevice.h"
#include "VulkanImage.h"
#include "graphics/Pipeline.h"
#include "graphics/Buffer.h"

#include <vulkan\vulkan.h>

namespace VulkanUtil
{
	using namespace Quartz;

	String				VendorNameFromID(UInt32 vendorID);

	ImageFormat			VkFormatToImageFormat(VkFormat vkFormat);

	VkFormat			ImageFormatToVkFormat(ImageFormat format);

	VkImageType			ImageTypeToVkImageType(ImageType type);

	VkImageUsageFlags	ImageUsagesToVkImageUsageFlags(ImageUsages usages);

	VkImageViewType		ImageViewTypeToVkImageViewType(ImageViewType type);

	VkImageAspectFlags	ImageUsageToVkImageAspects(ImageViewUsage usage);

	VkBufferUsageFlags		BufferUsagesToVkBufferUsageFlags (BufferUsages bufferUsages);

	VkMemoryPropertyFlags	BufferAccessToVkMemoryPropertyFlags(BufferAccess bufferAccess);

	VkPrimitiveTopology PrimitiveTopologyToVkPrimitiveTopology(PrimitiveTopology topology);

	VkPolygonMode		PolygonModeToVkPolygonMode(PolygonMode mode);

	VkCullModeFlags		CullModeToVkCullMode(CullMode mode);

	VkFrontFace			FaceWindToVkFrontFace(FaceWind wind);

	UInt32				MultisampleToVkMultisample(Multisample multisample);

	VkFormat			AttributeTypeToVkFormat(AttributeType type);

	UInt32				AttributeTypeToSize(AttributeType type);

	VkResult			EnumerateVkLayerProperties(Array<VkLayerProperties>& layerProperties);

	VkResult			EnumerateVkExtensionProperties(Array<VkExtensionProperties>& extensionProperties);

	VkResult			EnumerateVkPhysicalDevices(VkInstance vkInstance, Array<VkPhysicalDevice>& physicalDevices);

	UInt32				FindCompatableMemoryType(VulkanDevice* pDevice, Flags32 memoryTypeBits, VkMemoryPropertyFlags memoryProperties);

	VkResult			CreateVkInstance(VkInstance* pvkInstance, const char* appName,
							Array<const char*> validationLayers, Array<const char*> extentions);

}