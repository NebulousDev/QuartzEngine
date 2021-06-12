#include "VulkanFrameState.h"

namespace Quartz
{
	VulkanDescriptorPool::VulkanDescriptorPool(VkDescriptorPool vkDescriptorPool, 
		VulkanDevice* pDevice, UInt32 maxSetCount)
		: mvkDescriptorPool(vkDescriptorPool),
		mpDevice(pDevice),
		mMaxSetCount(maxSetCount)
	{
		// Nothing
	}
}

