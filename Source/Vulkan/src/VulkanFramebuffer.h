#pragma once

#include "Common.h"

#include "graphics\GFXFramebuffer.h"
#include "VulkanDevice.h"
#include "util\Array.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanFrameBuffer : public GFXFramebuffer
	{
	private:
		VulkanDevice*	mpParentDevice;
		VkFramebuffer	mFramebuffer;
		Bool8			mValidFrameBuffer;
		UInt32			mWidth;
		UInt32			mHeight;

	public:
		VulkanFrameBuffer() = default;
		VulkanFrameBuffer(VulkanDevice& vulkanDevice, VkRenderPass renderPass, 
			UInt32 width, UInt32 height, const Array<VkImageView>& attachemnts);

		VulkanDevice& GetParentDevice() { return *mpParentDevice; }

		VkFramebuffer GetFrameBufferHandle() { return mFramebuffer; }
		UInt32 GetWidth() const { return mWidth; }
		UInt32 GetHeight() const { return mHeight; }

		Bool8 IsValidFrameBuffer() const { return mValidFrameBuffer; }
	};
}