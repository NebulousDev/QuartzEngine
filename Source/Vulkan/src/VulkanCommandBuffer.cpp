#include "VulkanCommandBuffer.h"

#include "log\Log.h"

namespace Quartz
{
	VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice& device, VkCommandPool commandPool)
	{
		mpParentDevice = &device;

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(device.GetDeviceHandle(), &allocInfo, &mCommandBuffer) != VK_SUCCESS)
		{
			Log.Error("Failed to allocate command buffer: vkAllocateCommandBuffers failed!");
			return;
		}

		mCommandPool = commandPool;

		mValidCommandBuffer = true;
	}

	void VulkanCommandBuffer::Begin()
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(mCommandBuffer, &beginInfo) != VK_SUCCESS)
		{
			Log.Error("Failed to begin command buffer recording: vkBeginCommandBuffer failed!");
		}
	}

	void VulkanCommandBuffer::End()
	{
		if (vkEndCommandBuffer(mCommandBuffer) != VK_SUCCESS)
		{
			Log.Error("Failed to end command buffer recording: vkBeginCommandBuffer failed!");
		}
	}

	void VulkanCommandBuffer::Destroy()
	{
		vkFreeCommandBuffers(mpParentDevice->GetDeviceHandle(), mCommandPool, 1, &mCommandBuffer);
		mValidCommandBuffer = false;
	}
}

