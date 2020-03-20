#pragma once

#include "..\module\Module.h"
#include "..\platform\Window.h"

namespace Quartz
{
	/********************************************************************************************/


	class GFXModule;
	class GFXPhysicalDevice;
	class GFXSurface;

	class QUARTZ_API GFXPlatformModule : public Module
	{
	private:
		virtual Bool8		InitModuleImpl(const Char** pFailMessage) = 0;
		virtual Bool8		DestroyModuleImpl(const Char** pFailMessage) = 0;
		virtual const Char* GetModuleNameImpl() const = 0;

	public:
		virtual GFXSurface*		CreateSurface(GFXModule* pGFX, GFXDevice* pDevice, GFXSurfaceInfo info) = 0;
		virtual void			DestroySurface(GFXModule* pGFX, GFXSurface* pSurface) = 0;
		virtual GFXSwapchain*	CreateSwapchain(GFXDevice* pDevice, GFXSurface* pSurface, GFXSwapchainInfo info) = 0;
		virtual void			DestroySwapchain(GFXSwapchain* pSwapchain) = 0;

		virtual GFXImageArray	GetSwapChainImages(GFXSwapchain* pSwapchain) = 0;
	};


	/********************************************************************************************/
}