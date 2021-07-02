#include "VulkanViewport.h"

namespace Quartz
{
	VulkanContext::VulkanContext(VulkanSwapchain* pSwapchain, 
		Surface* pSurface, Scene* pScene, Renderer* pRenderer, MultibufferType multibuffer)
		: mpSwapchain(pSwapchain),
		Context(pSurface, pScene, pRenderer, multibuffer)
	{
		// Nothing
	}

	void VulkanContext::Rebuild()
	{
		//mSwapchain.Rebuild();
	}
}

