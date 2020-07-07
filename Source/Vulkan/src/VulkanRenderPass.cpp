#include "VulkanRenderPass.h"

#include "log/Log.h"

namespace Quartz
{
	VulkanRenderPass::VulkanRenderPass(VulkanDevice& device, VulkanRenderPassLayout& renderPassLayout)
	{
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.flags = 0;

		renderPassInfo.attachmentCount = renderPassLayout.attachments.Size();
		if (renderPassLayout.attachments.Size() > 0)
		{
			renderPassInfo.pAttachments = (VkAttachmentDescription*)renderPassLayout.attachments.Data();
		}

		Array<VkSubpassDescription> subpassDescriptions;
		for (const VulkanSubpass& subpass : renderPassLayout.subpasses)
		{
			VkSubpassDescription subpassDesc = {};
			subpassDesc.flags = 0;
			subpassDesc.pipelineBindPoint = subpass.pipelineBindPoint;

			subpassDesc.inputAttachmentCount = subpass.inputAttachments.Size();
			if (subpass.inputAttachments.Size() > 0)
			{
				subpassDesc.pInputAttachments = (const VkAttachmentReference*)subpass.inputAttachments.Data();
			}

			subpassDesc.colorAttachmentCount = subpass.colorAttachments.Size();
			if (subpass.colorAttachments.Size() > 0)
			{
				subpassDesc.pColorAttachments = (const VkAttachmentReference*)subpass.colorAttachments.Data();
			}

			if (subpass.resolveAttachments.Size() > 0)
			{
				subpassDesc.pResolveAttachments = (const VkAttachmentReference*)subpass.resolveAttachments.Data();
			}

			if (subpass.depthStencilAttachments.Size() > 0)
			{
				subpassDesc.pDepthStencilAttachment = (const VkAttachmentReference*)subpass.depthStencilAttachments.Data();
			}

			subpassDesc.preserveAttachmentCount = subpass.preserveAttachments.Size();
			if (subpass.preserveAttachments.Size() > 0)
			{
				subpassDesc.pPreserveAttachments = (const UInt32*)subpass.preserveAttachments.Data();
			}

			subpassDescriptions.PushBack(subpassDesc);
		}

		renderPassInfo.subpassCount = subpassDescriptions.Size();
		if (subpassDescriptions.Size() > 0)
		{
			renderPassInfo.pSubpasses = subpassDescriptions.Data();
		}

		renderPassInfo.dependencyCount = renderPassLayout.subpassDependencies.Size();
		if (renderPassLayout.subpassDependencies.Size() > 0)
		{
			renderPassInfo.pDependencies = (VkSubpassDependency*)renderPassLayout.subpassDependencies.Data();
		}

		if (vkCreateRenderPass(device.GetDeviceHandle(), &renderPassInfo, nullptr, &mRenderPass) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan render pass: vkCreateRenderPass failed!");
			return;
		}

		mpParentDevice = &device;
		mValidRenderPass = true;
	}

	void VulkanRenderPass::Destroy()
	{
		if (mValidRenderPass)
		{
			vkDestroyRenderPass(mpParentDevice->GetDeviceHandle(), mRenderPass, nullptr);
			mValidRenderPass = false;
		}
	}
}

