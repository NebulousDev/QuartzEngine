#pragma once

#include "Common.h"
#include "util\Array.h"
#include "util\Map.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class VulkanDevice;

	struct QUARTZ_API VulkanDeviceMemoryAllocation
	{
	private:
		VkDevice				mDevice;
		VkDeviceMemory			mDeviceMemory;
		VkMemoryPropertyFlags	mMemoryProperties;
		Flags32					mMemoryTypeBits;
		VkDeviceSize			mSizeBytes;

		void* mpBuffer;
		Bool8 mMapped;

	public:
		VulkanDeviceMemoryAllocation(VkDevice device, VkDeviceMemory deviceMemory,
			VkMemoryPropertyFlags memoryProperties, Flags32 memoryTypeBits, VkDeviceSize sizeBytes);

		void* MapMemory(UInt64 offsetBytes, UInt64 sizeBytes);
		void UnmapMemory();

		VkDeviceMemory GetDeviceMemoryHandle() const { return mDeviceMemory; }
		VkMemoryPropertyFlags GetMemoryProperties() const { return mMemoryProperties; }
		Flags32 GetMemoryTypeBits() const { return mMemoryTypeBits; }
		VkDeviceSize GetSize() const { return mSizeBytes; }

		void* GetRawBufferPointer() const { return mpBuffer; }

		Bool8 IsMapped() const { return mMapped; }
	};

	typedef Handle64 VulkanDeviceMemoryHandle;

	class QUARTZ_API VulkanDeviceMemoryAllocator
	{
	private:
		VulkanDevice*							mpParentDevice;
		Array<VulkanDeviceMemoryAllocation*>	mDeviceHeapAllocations;
		Map<VulkanDeviceMemoryHandle, UInt32>	mAllocationIndexMap;
		UInt64									mTotalAllocatedSize;

	private:
		UInt32 FindCompatableMemoryType(Flags32 memoryTypeBits, VkMemoryPropertyFlags memoryProperties);

	public:
		VulkanDeviceMemoryAllocator() = default;
		VulkanDeviceMemoryAllocator(VulkanDevice& device);

		VulkanDeviceMemoryAllocation* Allocate(VkDeviceSize sizeBytes, Flags32 memoryTypeBits, VkMemoryPropertyFlags memoryProperties);
	
		void Free(VulkanDeviceMemoryAllocation* deviceAllocation);
	};
}