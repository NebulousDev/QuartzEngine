#pragma once

#include "gfx\GraphicsSurface.h"
#include "VulkanImageView.h"
#include "VulkanDevice.h"
#include "PlatformWindow.h"

namespace Quartz
{
	class QUARTZ_API VulkanSurface : public GraphicsSurface
	{
	private:
		VkSurfaceKHR	mSurface;
		VkSwapchainKHR	mSwapChain;

		UInt32			mWidth;
		UInt32			mHeight;

		Window*			mpWindow;
		VulkanDevice*	mpDevice;

		VkFormat				mFormat;
		Array<VkImage>			mImages;
		Array<VulkanImageView>	mImageViews;
		Int64					mImageIndex;

		Array<VkSemaphore>	mImageAcquiredSemaphores;
		Array<VkSemaphore>	mRenderCompleteSemaphores;
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

		Int32 AcquireNextImageIndex();
	
		const VkSurfaceKHR& GetSurfaceHandle() const { return mSurface; }
		const VkSwapchainKHR& GetSwapChainHandle() const { return mSwapChain; }

		UInt32 GetWidth() const { return mWidth; }
		UInt32 GetHeight() const { return mHeight; }

		Window& GetRelativeWindow() { return *mpWindow; }
		VulkanDevice& GetDevice() { return *mpDevice; }

		VkFormat GetFormat() const { return mFormat; }
		const Array<VkImage>& GetImages() const { return mImages; }
		const Array<VulkanImageView>& GetImageViews() const { return mImageViews; }

		Bool8 IsFullscreen() const { return mIsFullscreen; }
		Bool8 IsVSynced() const { return mIsVSynced; }

		Bool8 IsValidSurface() const { return mIsValidSurface; }
	};
}