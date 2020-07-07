#pragma once

#include "graphics\GFXCommandBuffer.h"
#include "VulkanDevice.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanCommandBuffer : public GFXCommandBuffer
	{
	private:
		VulkanDevice*	mpParentDevice;
		VkCommandBuffer mCommandBuffer;
		Bool8			mValidCommandBuffer;

	public:
		VulkanCommandBuffer(VulkanDevice& device, VkCommandPool commandPool);

		void Begin() override;
		void End() override;

		VulkanDevice& GetParentDevice() { return *mpParentDevice; }
		VkCommandBuffer GetCommandBufferHandle() const { return mCommandBuffer; }
		Bool8 IsValidCommandBuffer() const { return mValidCommandBuffer; }
	};
}