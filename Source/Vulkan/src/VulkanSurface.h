#pragma once

#include "graphics\GFXSurface.h"
#include "util\Array.h"
#include "VulkanImage.h"
#include "VulkanDevice.h"
#include "PlatformWindow.h"

namespace Quartz
{
	class QUARTZ_API VulkanSurface : public GFXSurface
	{
	private:
		VkSurfaceKHR	mSurface;
		VkSwapchainKHR	mSwapChain;

		Window*			mpWindow;
		VulkanDevice*	mpDevice;

		VkFormat				mFormat;
		Array<VkImage>			mImages;
		Array<VulkanImageView>	mImageViews;
		Int32					mImageIndex;

		Array<VkSemaphore>	mImageAcquiredSemaphores;
		Array<VkSemaphore>	mImageCompleteSemaphores;
		Array<VkFence>		mImageFences;

		struct
		{
			Bool8 mIsFullscreen : 1;
			Bool8 mIsVSynced : 1;
			Bool8 mIsValidSurface : 1;
		};

	private:
		Bool8 CreateSurface(VkInstance instance, Window& window);
		Bool8 CreateSwapChain(VkInstance instance, VulkanDevice& device, VkSurfaceKHR surface,
			UInt32 bufferCount, UInt32 width, UInt32 height, Bool8 fullscreen, UInt32 vSync);
		Bool8 CreateImageViews(VulkanDevice& device, VkFormat format);
		Bool8 CreateSemaphores();

	public:
		VulkanSurface() = default;

		VulkanSurface(VkInstance instance, Window& window, VulkanDevice& device, 
			UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen);

		UInt32 AcquireNextImageIndex() override;
	
		const VkSurfaceKHR& GetSurfaceHandle() const { return mSurface; }
		const VkSwapchainKHR& GetSwapChainHandle() const { return mSwapChain; }

		Window& GetRelativeWindow() { return *mpWindow; }
		VulkanDevice& GetDevice() { return *mpDevice; }

		VkSemaphore GetImageAqcuiredSemaphore(UInt32 index) { return mImageAcquiredSemaphores[index]; }
		VkSemaphore GetImageCompleteSemaphore(UInt32 index) { return mImageCompleteSemaphores[index]; }
		VkFence GetImageFence(UInt32 index) { return mImageFences[index]; }

		VkFormat GetFormat() const { return mFormat; }
		Array<VkImage>& GetImages() { return mImages; }
		Array<VulkanImageView>& GetImageViews() { return mImageViews; }

		UInt32 GetImageIndex() const { return mImageIndex; }

		Bool8 IsFullscreen() const { return mIsFullscreen; }
		Bool8 IsVSynced() const { return mIsVSynced; }

		Bool8 IsValidSurface() const { return mIsValidSurface; }
	};
}