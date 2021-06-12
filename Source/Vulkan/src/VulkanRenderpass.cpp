#include "VulkanRenderpass.h"

namespace Quartz
{
	VulkanRenderpass::VulkanRenderpass(VkRenderPass vkRenderpass, const String& name, 
		const Array<Attachment>& attachments, const Array<Subpass>& subpasses)
		: mvkRenderpass(vkRenderpass),
		Renderpass(name, attachments, subpasses)
	{
		// Nothing
	}
}

