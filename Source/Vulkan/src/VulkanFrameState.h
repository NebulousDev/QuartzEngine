#pragma once

#include "VulkanPipeline.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanDescriptorPool
	{
	private:
		VkDescriptorPool	mvkDescriptorPool;
		VulkanDevice*		mpDevice;
		UInt32				mMaxSetCount;

	public:
		VulkanDescriptorPool(VkDescriptorPool vkDescriptorPool, 
			VulkanDevice* pDevice, UInt32 maxSetCount);

		void AllocateDesciptorSets(VkDescriptorSetLayout vkDescriptorSetLayout, 
			Array<VkWriteDescriptorSet>& outWrites, Array<VkDescriptorSet> outSets);

		FORCE_INLINE VkDescriptorPool	GetVkDescriptorPool() { return mvkDescriptorPool; }
		FORCE_INLINE VulkanDevice*		GetDevice() { return mpDevice;}
		FORCE_INLINE UInt32				GetMaxSetCount() { return mMaxSetCount; }
	};

	class QUARTZ_API VulkanDescriptorPoolCache
	{

	};

	struct VulkanDescriptorSetCacheKey
	{

	};

	class QUARTZ_API VulkanDescriptorSetCache
	{
	private:

	public:
		void GetOrCreateDescriptorSet(UInt32 set);
	};
}