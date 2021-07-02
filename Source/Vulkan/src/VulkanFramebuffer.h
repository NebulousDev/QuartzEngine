#pragma once

#include "graphics/Framebuffer.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanFramebuffer : public Framebuffer
	{
	private:
		Array<VkFramebuffer> mvkFramebuffers;

	public:
		VulkanFramebuffer(const Array<VkFramebuffer>& vkFramebuffers, const Array<Image*>& images,
			const Array<ImageView*>& imageViews, UInt32 width, UInt32 height);

		FORCE_INLINE const Array<VkFramebuffer>&	GetVkFramebuffers() const { return mvkFramebuffers; }
		FORCE_INLINE const UInt32					GetFramebufferCount() const { return mvkFramebuffers.Size(); }
	};
}