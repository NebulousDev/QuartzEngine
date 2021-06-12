#pragma once

#include "graphics2/RenderPass.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanRenderpass : public Renderpass
	{
	private:
		VkRenderPass mvkRenderpass;

	public:
		VulkanRenderpass(VkRenderPass vkRenderpass, const String& name,
			const Array<Attachment>& attachments, const Array<Subpass>& subpasses);

		FORCE_INLINE VkRenderPass GetVkRenderPass() { return mvkRenderpass; }
	};
}