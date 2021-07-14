#pragma once

#include "Common.h"
#include <vulkan/vulkan.h>

namespace Quartz
{
	// TODO: Improve
	class VulkanSampler
	{
	private:
		VkSampler mvkSampler;

	public:
		VulkanSampler(VkSampler vkSampler);

		FORCE_INLINE VkSampler GetVkSampler() { return mvkSampler; }
	};
}