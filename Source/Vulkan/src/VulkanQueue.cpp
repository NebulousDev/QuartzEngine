#include "VulkanQueue.h"

#include "VulkanDevice.h"

namespace Quartz
{
	VulkanQueue::VulkanQueue(VulkanDevice& device, UInt32 familyIndex, VkQueueFamilyProperties queueFamilyProperties)
	{
		mpParentDevice = &device;
		mFamilyIndex = familyIndex;
		mQueueIndex = 0; //???

		vkGetDeviceQueue(device.GetDeviceHandle(), familyIndex, 0, &mQueue);

		if ((queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT)
		{
			mSupportsGraphics = true;
		}

		if ((queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT)
		{
			mSupportsTransfer = true;
		}

		if ((queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT)
		{
			mSupportsCompute = true;
		}

		mSupportsPresent = false; //???
	}
}

