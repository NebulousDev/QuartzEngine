#include "VulkanFramebuffer.h"

#include "log\Log.h"

namespace Quartz
{
	VulkanFrameBuffer::VulkanFrameBuffer(VulkanDevice& vulkanDevice, VkRenderPass renderPass, UInt32 width, UInt32 height, const Array<VkImageView>& attachemnts)
	{
		mpParentDevice = &vulkanDevice;
		mWidth = width;
		mHeight = height;

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = renderPass;
		framebufferInfo.attachmentCount = attachemnts.Size();
		framebufferInfo.pAttachments = attachemnts.Data();
		framebufferInfo.width = width;
		framebufferInfo.height = height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(vulkanDevice.GetDeviceHandle(), &framebufferInfo, nullptr, &mFramebuffer) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan frame buffer: vkCreateFramebuffer failed!");
			return;
		}

		mValidFrameBuffer = true;
	}
}

