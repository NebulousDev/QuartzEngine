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
		VkCommandPool	mCommandPool;
		Bool8			mValidCommandBuffer;

	public:
		VulkanCommandBuffer(VulkanDevice& device, VkCommandPool commandPool);

		void Begin() override;
		void End() override;

		void Destroy();

		VulkanDevice& GetParentDevice() { return *mpParentDevice; }
		VkCommandBuffer GetCommandBufferHandle() const { return mCommandBuffer; }
		Bool8 IsValidCommandBuffer() const { return mValidCommandBuffer; }
	};
}