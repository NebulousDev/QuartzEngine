#pragma once

#include "VulkanSwapchain.h"
#include "graphics2/Viewport.h"

#include "VulkanBuffer.h"

namespace Quartz
{
	class VulkanMultibufferdResourceManager
	{
	private:
		Array<Array<VulkanBuffer>> mBufferStore;
	};

	class QUARTZ_API VulkanContext : public Context
	{
	private:
		VulkanSwapchain* mpSwapchain;

	public:
		VulkanContext(VulkanSwapchain* pSwapchain, 
			Surface* pSurface, Scene* pScene, Renderer* pRenderer, MultibufferType multibuffer);

		void Rebuild();

		FORCE_INLINE VulkanSwapchain*	GetSwapchain() { return mpSwapchain; }
		FORCE_INLINE UInt32				GetCurrentFrame() { return mpSwapchain->GetCurrentFrame(); }
		FORCE_INLINE UInt32				GetFrameIndex() { return mpSwapchain->GetFrameIndex(); }
	};
}