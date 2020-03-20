#pragma once

#include "gfx\GFXContext.h"
#include "gfx\GFXPlatform.h"
#include "vulkan\vulkan.h"

#include "util\Array.h"

namespace Quartz
{
	/********************************************************************************************/


	class QUARTZ_API VulkanGFXSurface : public GFXSurface
	{
	private:
		VkSurfaceKHR				mVkSurface;
		VkSurfaceCapabilitiesKHR	mVkSurfaceCapabilities;
		Array<VkSurfaceFormatKHR>	mVkSurfaceFormats;
		Array<VkPresentModeKHR>		mVkPresentModes;

	public:
		VulkanGFXSurface(VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR capibilities, 
			const Array<VkSurfaceFormatKHR>& surfaceFormats, const Array<VkPresentModeKHR>& presentModes);

		FORCE_INLINE VkSurfaceKHR						GetVkSurface() const { return mVkSurface; }
		FORCE_INLINE VkSurfaceCapabilitiesKHR			GetVkSurfaceCapibilities() const { return mVkSurfaceCapabilities; }
		FORCE_INLINE const Array<VkSurfaceFormatKHR>&	GetVkSurfaceFormats() const { return mVkSurfaceFormats; }
		FORCE_INLINE const Array<VkPresentModeKHR>&		GetVkPresentModes() const { return mVkPresentModes; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXSwapchain : public GFXSwapchain
	{
	private:
		VkSwapchainKHR		mVkSwapchain;
		VkSurfaceFormatKHR	mVkSurfaceFormat;

	public:
		VulkanGFXSwapchain(GFXDevice* pDevice, GFXSurface* pSurface, VkSwapchainKHR swapchain,
			VkSurfaceFormatKHR surfaceFormat, GFXSwapchainInfo info);

		FORCE_INLINE VkSwapchainKHR		GetVkSwapchain() const { return mVkSwapchain; }
		FORCE_INLINE VkSurfaceFormatKHR GetVkSurfaceFormat() const { return mVkSurfaceFormat; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanWin32GFXModule : public GFXPlatformModule
	{
	private:
		Bool8		InitModuleImpl(const Char** pFailMessage) final override;
		Bool8		DestroyModuleImpl(const Char** pFailMessage) final override;
		const Char* GetModuleNameImpl() const final override;

	public:
		GFXSurface*		CreateSurface(GFXModule* pGFX, GFXDevice* pDevice, GFXSurfaceInfo info) final override;
		void			DestroySurface(GFXModule* pGFX, GFXSurface* pSurface) final override;
		GFXSwapchain*	CreateSwapchain(GFXDevice* pDevice, GFXSurface* pSurface, GFXSwapchainInfo info) final override;
		void			DestroySwapchain(GFXSwapchain* pSwapchain) final override;

		GFXImageArray	GetSwapChainImages(GFXSwapchain* pSwapchain) final override;
	};


	/********************************************************************************************/
}