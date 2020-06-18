#include "VulkanQueue.h"

#include "VulkanDevice.h"

namespace Quartz
{
	VulkanQueue::VulkanQueue(VulkanDevice* pDevice, UInt32 familyIndex)
	{
		vkGetDeviceQueue(pDevice->GetDeviceHandle(), familyIndex, 0, &mQueue);
	}
}

