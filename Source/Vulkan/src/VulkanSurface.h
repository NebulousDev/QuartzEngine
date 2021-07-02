#pragma once

#include "graphics/Surface.h"

#include "util/Array.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanSurface : public Surface
	{
	private:
		VkSurfaceKHR				mvkSurface;
		Array<VkSurfaceFormatKHR>	mSupportedFormats;
		VkSurfaceCapabilitiesKHR	mCapibilites;

	public:
		VulkanSurface(VkSurfaceKHR vkSurface, UInt32 width, UInt32 height, 
			const Array<VkSurfaceFormatKHR>& formats, VkSurfaceCapabilitiesKHR capibilites, 
			Bool8 supportsHDR);

		FORCE_INLINE VkSurfaceKHR				GetVkSurface() { return mvkSurface; };
		FORCE_INLINE const Array<VkSurfaceFormatKHR>& 
												GetSupportedFormats() const { return mSupportedFormats; }
		FORCE_INLINE VkSurfaceCapabilitiesKHR	GetCapibilites() { return mCapibilites; }
	};
}