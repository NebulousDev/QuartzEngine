#pragma once

#include "VulkanImage.h"
#include "VulkanDevice.h"

#include "util/Array.h"

namespace Quartz
{
	class QUARTZ_API VulkanSwapchain
	{
	private:
		VkSwapchainKHR			mvkSwapchain;
		VulkanDevice*			mpDevice;
		UInt32					mBackbufferCount;

		Array<VulkanImage*>		mImages;
		Array<VulkanImageView*>	mImageViews;
		Array<VkSemaphore>		mImageAvailableSemaphores;
		Array<VkSemaphore>		mImageCompleteSemaphores;
		Array<VkFence>			mImageFences;
		Array<VkFence>			mInFlightFences;

		UInt32					mImageIndex;
		UInt32					mCurrentFrame;

	public:
		VulkanSwapchain(VkSwapchainKHR vkSwapchain,
			VulkanDevice* pDevice,
			UInt32 backbufferCount,
			const Array<VulkanImage*>& images,
			const Array<VulkanImageView*>& imageViews,
			const Array<VkSemaphore>& imageAcquiredSemaphores,
			const Array<VkSemaphore>& imageCompleteSemaphores,
			const Array<VkFence>& imageFences,
			const Array<VkFence>& inFlightFences);

		void Rebuild();

		void AdvanceFrame();
		void Present();

		ImageView*	GetCurrentImageView();
		VkSemaphore GetCurrentAcquiredSemaphore();
		VkSemaphore GetCurrentCompleteSemaphore();
		VkFence		GetCurrentFence();

		FORCE_INLINE UInt32							GetFrameIndex() { return mImageIndex; }
		FORCE_INLINE UInt32							GetCurrentFrame() { return mCurrentFrame; }

		FORCE_INLINE UInt32							GetBackbufferCount() { return mBackbufferCount; }
		FORCE_INLINE VkSwapchainKHR					GetVkSwapchain() { return mvkSwapchain; }
		FORCE_INLINE const Array<VulkanImage*>&		GetImages() const { return mImages; }
		FORCE_INLINE const Array<VulkanImageView*>& GetImageViews() const { return mImageViews; }
	};
}