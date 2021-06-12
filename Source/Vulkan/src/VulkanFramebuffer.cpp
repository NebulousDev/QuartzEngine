#include "VulkanFramebuffer.h"

namespace Quartz
{
	VulkanFramebuffer::VulkanFramebuffer(const Array<VkFramebuffer>& vkFramebuffers, const Array<Image*>& images,
		const Array<ImageView*>& imageViews, UInt32 width, UInt32 height)
		: mvkFramebuffers(vkFramebuffers),
		Framebuffer(images, imageViews, width, height)
	{
		// Nothing
	}
}

