#pragma once

#include "graphics2/Buffer.h"
#include "VulkanDevice.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanBuffer : public Buffer
	{
	private:
		VkBuffer				mvkBuffer;
		VulkanDevice*			mpDevice;
		VkDeviceMemory			mvkMemory;
		VkBufferUsageFlags		mvkUsage;
		VkMemoryPropertyFlags	mvkMemoryProperties;

	public:
		VulkanBuffer(VkBuffer vkBuffer, VulkanDevice* pDevice, VkBufferUsageFlags vkUsage, VkDeviceMemory vkMemory,
			VkMemoryPropertyFlags vkMemoryProperties, UInt32 size, BufferUsages usages, BufferAccess access);

		void*	MapBuffer(UInt32 sizeBytes, UInt32 offsetBytes) override;
		void	UnmapBuffer() override;

		FORCE_INLINE VkBuffer				GetVkBuffer() { return mvkBuffer; }
		FORCE_INLINE VkDeviceMemory			GetVkMemory() { return mvkMemory; }
		FORCE_INLINE VkBufferUsageFlags		GetVkUsage() { return mvkUsage; }
		FORCE_INLINE VkMemoryPropertyFlags	GetVkMemoryProperties() { return mvkMemoryProperties; }
	};
}