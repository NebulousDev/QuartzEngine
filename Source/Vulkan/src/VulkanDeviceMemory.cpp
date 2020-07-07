#include "VulkanDeviceMemory.h"
#include "VulkanDevice.h"

#include "log/log.h"

namespace Quartz
{
	VulkanDeviceMemoryAllocation::VulkanDeviceMemoryAllocation(VkDevice device, VkDeviceMemory deviceMemory, VkMemoryPropertyFlags memoryProperties, Flags32 memoryTypeBits, VkDeviceSize sizeBytes)
		: mDevice(device), mDeviceMemory(deviceMemory), mMemoryProperties(memoryProperties), 
		mMemoryTypeBits(memoryTypeBits), mSizeBytes(sizeBytes), mpBuffer(nullptr), mMapped(false)
	{

	}

	void* VulkanDeviceMemoryAllocation::MapMemory(UInt64 offsetBytes, UInt64 sizeBytes)
	{
		if (!mMapped)
		{
			if (sizeBytes - offsetBytes > mSizeBytes)
			{
				Log.Error("Unable to map device memory: offset + size is out of bounds!");
				return nullptr;
			}

			if (vkMapMemory(mDevice, mDeviceMemory, offsetBytes, sizeBytes, 0, &mpBuffer) != VK_SUCCESS)
			{
				Log.Error("Unable to map device memory: vkMapMemory failed!");
				return nullptr;
			}

			mMapped = true;
		}
		
		return mpBuffer;
	}

	void VulkanDeviceMemoryAllocation::UnmapMemory()
	{
		if (mMapped)
		{
			vkUnmapMemory(mDevice, mDeviceMemory);
			mpBuffer = nullptr;
			mMapped = false;
		}
	}

	UInt32 VulkanDeviceMemoryAllocator::FindCompatableMemoryType(Flags32 memoryTypeBits, VkMemoryPropertyFlags memoryProperties)
	{
		VkPhysicalDeviceMemoryProperties deviceMemoryProperties = mpParentDevice->GetPhysicalDevice().GetPhysicalDeviceMemoryProperties();

		for (UInt32 i = 0; i < deviceMemoryProperties.memoryTypeCount; i++)
		{
			if ((memoryTypeBits & (1 << i)) && (deviceMemoryProperties.memoryTypes[i].propertyFlags & memoryProperties) == memoryProperties)
			{
				return i;
			}
		}

		return (UInt32)-1;
	}

	VulkanDeviceMemoryAllocator::VulkanDeviceMemoryAllocator(VulkanDevice& device)
		: VulkanDeviceMemoryAllocator()
	{
		mpParentDevice = &device;
	}

	VulkanDeviceMemoryAllocation* VulkanDeviceMemoryAllocator::Allocate(VkDeviceSize sizeBytes, Flags32 memoryTypeBits, VkMemoryPropertyFlags memoryProperties)
	{
		VkMemoryAllocateInfo allocateInfo = {};
		allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocateInfo.allocationSize = sizeBytes;

		UInt32 compatableMemoryTypeIndex = FindCompatableMemoryType(memoryTypeBits, memoryProperties);

		if (compatableMemoryTypeIndex == (UInt32)-1)
		{
			Log.Critical("Failed to allocate device memory: Unable to find a compatable memory type index!");
			return nullptr;
		}

		allocateInfo.memoryTypeIndex = compatableMemoryTypeIndex;

		VkDeviceMemory deviceMemory;
		if (vkAllocateMemory(mpParentDevice->GetDeviceHandle(), &allocateInfo, nullptr, &deviceMemory) != VK_SUCCESS)
		{
			Log.Critical("Failed to allocate device memory: vkAllocateMemory failed!");
			return nullptr;
		}

		VulkanDeviceMemoryAllocation allocation(mpParentDevice->GetDeviceHandle(), deviceMemory, memoryProperties, memoryTypeBits, sizeBytes);
		VulkanDeviceMemoryAllocation* pStoredAllocation = mDeviceHeapAllocations.PushBack(allocation);
		mAllocationIndexMap.Put((VulkanDeviceMemoryHandle)pStoredAllocation, mDeviceHeapAllocations.Size() - 1);

		mTotalAllocatedSize += sizeBytes;

		return pStoredAllocation;
	}

	void VulkanDeviceMemoryAllocator::Free(VulkanDeviceMemoryAllocation* deviceAllocation)
	{
		//TODO
	}
	
}

