#include "VulkanSurface.h"

namespace Quartz
{
	VulkanSurface::VulkanSurface(VkSurfaceKHR vkSurface, UInt32 width, UInt32 height, 
		const Array<VkSurfaceFormatKHR>& formats, VkSurfaceCapabilitiesKHR capibilites, 
		Bool8 supportsHDR)
		: mvkSurface(vkSurface),
		mSupportedFormats(formats),
		mCapibilites(capibilites),
		Surface(width, height, supportsHDR)
	{
		// Nothing
	}
}

