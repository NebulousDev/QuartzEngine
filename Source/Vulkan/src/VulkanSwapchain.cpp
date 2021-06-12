#include "VulkanSwapchain.h"

#include "Engine.h"

namespace Quartz
{
	VulkanSwapchain::VulkanSwapchain(VkSwapchainKHR vkSwapchain,
		VulkanDevice* pDevice,
		UInt32 backbufferCount,
		const Array<VulkanImage*>& images, 
		const Array<VulkanImageView*>& imageViews, 
		const Array<VkSemaphore>& imageAvailableSemaphores, 
		const Array<VkSemaphore>& imageCompleteSemaphores, 
		const Array<VkFence>& imageFences, 
		const Array<VkFence>& inFlightFences)
		: mvkSwapchain(vkSwapchain),
		mpDevice(pDevice),
		mBackbufferCount(backbufferCount),
		mImages(images),
		mImageViews(imageViews),
		mImageAvailableSemaphores(imageAvailableSemaphores),
		mImageCompleteSemaphores(imageCompleteSemaphores),
		mImageFences(imageFences),
		mInFlightFences(inFlightFences),
		mCurrentFrame(-1),
		mImageIndex(0)
	{
		// Nothing
	}

	void VulkanSwapchain::Rebuild()
	{

	}

	#define FENCE_TIMEOUT UINT64_MAX //1000000 // UINT64_MAX

	void VulkanSwapchain::AdvanceFrame()
	{
		// @NOTE: doesn't have to be backbuffer count?
		mCurrentFrame = (mCurrentFrame + 1) % mBackbufferCount;

		vkWaitForFences(mpDevice->GetDeviceHandle(), 1, &mInFlightFences[mCurrentFrame], VK_TRUE, FENCE_TIMEOUT);

		vkAcquireNextImageKHR(mpDevice->GetDeviceHandle(), mvkSwapchain, 
			UINT64_MAX, mImageAvailableSemaphores[mCurrentFrame], VK_NULL_HANDLE, &mImageIndex);

		if (mImageFences[mImageIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(mpDevice->GetDeviceHandle(), 1, &mImageFences[mImageIndex], VK_TRUE, FENCE_TIMEOUT);
		}

		mImageFences[mImageIndex] = mInFlightFences[mCurrentFrame];

		vkResetFences(mpDevice->GetDeviceHandle(), 1, &mInFlightFences[mCurrentFrame]);
	}

	void VulkanSwapchain::Present()
	{
		VkSemaphore		signalSemaphores[]	= { mImageCompleteSemaphores[mCurrentFrame] };
		VkSwapchainKHR	swapChains[]		= { mvkSwapchain };
		UInt32			imageIndices[]		= { mImageIndex };

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount	= 1;
		presentInfo.pWaitSemaphores		= signalSemaphores;
		presentInfo.swapchainCount		= 1;
		presentInfo.pSwapchains			= swapChains;
		presentInfo.pImageIndices		= imageIndices;
		presentInfo.pResults			= nullptr;

		VulkanQueue& presentQueue = mpDevice->GetPresentQueue();

		vkQueuePresentKHR(presentQueue.GetQueueHandle(), &presentInfo);
	}

	ImageView* VulkanSwapchain::GetCurrentImageView()
	{
		return mImageViews[mCurrentFrame];
	}

	VkSemaphore VulkanSwapchain::GetCurrentAcquiredSemaphore()
	{
		return mImageAvailableSemaphores[mCurrentFrame];
	}

	VkSemaphore VulkanSwapchain::GetCurrentCompleteSemaphore()
	{
		return mImageCompleteSemaphores[mCurrentFrame];
	}

	VkFence VulkanSwapchain::GetCurrentFence()
	{
		return mImageFences[mCurrentFrame];
	}
}

