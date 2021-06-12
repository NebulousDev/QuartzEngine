#include "VulkanBuffer.h"

#include "log/Log.h"

namespace Quartz
{
	VulkanBuffer::VulkanBuffer(VkBuffer vkBuffer, VulkanDevice* pDevice, VkBufferUsageFlags vkUsage, VkDeviceMemory vkMemory,
		VkMemoryPropertyFlags vkMemoryProperties, UInt32 size, BufferUsages usages, BufferAccess access)
		: mvkBuffer(vkBuffer),
		mpDevice(pDevice),
		mvkUsage(vkUsage),
		mvkMemory(vkMemory),
		mvkMemoryProperties(vkMemoryProperties),
		Buffer(size, usages, access)
	{
		// Nothing
	}

	void* VulkanBuffer::MapBuffer(UInt32 sizeBytes, UInt32 offsetBytes)
	{
		void* pMapData = nullptr;

		if (!mIsMapped)
		{
			if (sizeBytes - offsetBytes > mSize)
			{
				Log::Error("Unable to map device memory: offset + size is out of bounds!");
				return nullptr;
			}

			if (vkMapMemory(mpDevice->GetDeviceHandle(), mvkMemory, offsetBytes, sizeBytes, 0, &pMapData) != VK_SUCCESS)
			{
				Log::Error("Unable to map device memory: vkMapMemory failed!");
				return nullptr;
			}

			mIsMapped = true;
		}

		return pMapData;
	}

	void VulkanBuffer::UnmapBuffer()
	{
		if (mIsMapped)
		{
			vkUnmapMemory(mpDevice->GetDeviceHandle(), mvkMemory);
			mIsMapped = false;
		}
	}
}

