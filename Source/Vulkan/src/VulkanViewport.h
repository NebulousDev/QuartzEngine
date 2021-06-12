#pragma once

#include "VulkanSwapchain.h"
#include "graphics2/Viewport.h"

namespace Quartz
{
	class QUARTZ_API VulkanViewport : public Viewport
	{
	private:
		VulkanSwapchain* mpSwapchain;

	public:
		VulkanViewport(VulkanSwapchain* pSwapchain, Surface* pSurface, Scene* pScene, Renderer* pRenderer);

		void Rebuild();

		FORCE_INLINE VulkanSwapchain* GetSwapchain() { return mpSwapchain; }
	};
}