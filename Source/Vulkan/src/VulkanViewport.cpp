#include "VulkanViewport.h"

namespace Quartz
{
	VulkanViewport::VulkanViewport(VulkanSwapchain* pSwapchain, Surface* pSurface, Scene* pScene, Renderer* pRenderer)
		: mpSwapchain(pSwapchain),
		Viewport(pSurface, pScene, pRenderer)
	{
		// Nothing
	}

	void VulkanViewport::Rebuild()
	{
		//mSwapchain.Rebuild();
	}
}

