#include "WindowManager.h"

#include "../Engine.h"

namespace Quartz
{
	GraphicsWindow* WindowManager::CreateGraphicsWindow(UInt32 posX, UInt32 posY, UInt32 width, UInt32 height, const StringW& title, UInt32 backbufferCount)
	{
		HVPWindow hWindow = Engine::GetPlatformContext().CreateWindow(posX, posY, width, height, title);
		HGFXSurface hSurface = Engine::GetGraphicsContext().CreateSurface(hWindow, width, height, false, false);
		HGFXSwapchain hSwapchain = Engine::GetGraphicsContext().CreateSwapchain(hSurface, backbufferCount, width, height, false, false);
	
		GraphicsWindow* pGraphicsWindow = new GraphicsWindow;
		pGraphicsWindow->window = hWindow;
		pGraphicsWindow->surface = hSurface;
		pGraphicsWindow->swapchain = hSwapchain;

		return pGraphicsWindow;
	}

	void WindowManager::DestroyGraphicsWindow(GraphicsWindow* pWindow)
	{
		Engine::GetPlatformContext().DestroyWindow(pWindow->window);
		Engine::GetGraphicsContext().DestroySwapchain(pWindow->swapchain);
		Engine::GetGraphicsContext().DestroySurface(pWindow->surface);

		delete pWindow;
	}
}

