#pragma once

#include "VulkanDevice.h"
#include "graphics\GFXRenderPass.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	struct QUARTZ_API VulkanAttachment
	{
		VkAttachmentDescriptionFlags flags;
		VkFormat format;
		VkSampleCountFlagBits samples;
		VkAttachmentLoadOp loadOp;
		VkAttachmentStoreOp storeOp;
		VkAttachmentLoadOp stencilLoadOp;
		VkAttachmentStoreOp stencilStoreOp;
		VkImageLayout initialLayout;
		VkImageLayout finalLayout;
	};

	struct QUARTZ_API VulkanAttachmentReference
	{
		UInt32 attachment;
		VkImageLayout layout;
	};

	struct QUARTZ_API VulkanSubpass
	{
		VkPipelineBindPoint pipelineBindPoint;
		Array<VulkanAttachmentReference> inputAttachments;
		Array<VulkanAttachmentReference> colorAttachments;
		Array<VulkanAttachmentReference> resolveAttachments;
		Array<VulkanAttachmentReference> depthStencilAttachments;
		Array<UInt32> preserveAttachments;
	};

	struct QUARTZ_API VulkanSubpassDependency
	{
		uint32_t srcSubpass;
		uint32_t dstSubpass;
		VkPipelineStageFlags srcStageMask;
		VkPipelineStageFlags dstStageMask;
		VkAccessFlags srcAccessMask;
		VkAccessFlags dstAccessMask;
		VkDependencyFlags dependencyFlags;
	};

	struct QUARTZ_API VulkanRenderPassLayout
	{
		Array<VulkanAttachment> attachments;
		Array<VulkanSubpass> subpasses;
		Array<VulkanSubpassDependency> subpassDependencies;
	};

	class QUARTZ_API VulkanRenderPass : public GFXRenderPass
	{
	private:
		VulkanDevice* mpParentDevice;
		VkRenderPass mRenderPass;
		Bool8 mValidRenderPass;

	public:
		VulkanRenderPass() = default;
		VulkanRenderPass(VulkanDevice& device, VulkanRenderPassLayout& renderPassLayout);

		void Destroy();

		VulkanDevice& GetParentDevice() { return *mpParentDevice; }
		VkRenderPass GetRenderPassHandle() const { return mRenderPass; }

		Bool8 IsValidRenderPass() const { return mValidRenderPass; }
	};
}