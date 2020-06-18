// Sandbox Main

#include "Common.h"

#include "Win32Platform.h"
#include "Engine.h"

#include "util\Map.h"
#include "util\Array.h"

#include "VulkanContext.h"
#include "VulkanSwapChain.h"
#include "VulkanSurface.h"
#include "gfx\GraphicsUtil.h"

using namespace Quartz;

int main()
{
	/* Initialize Platform Layers */
	Platform* pPlatform = new Win32Platform();

	/* Initialize Engine */
	Engine& engine = Engine::GetInstance();
	engine.SetPlatform(pPlatform);
	
	engine.Init();

	/* Create Window */
	Window* pWindow = engine.GetPlatform().CreateWindow(640, 480, "Quartz", 0);
	pWindow->Show();

	/* Graphics Context */
	Array<String> extensions;
	extensions.PushBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	extensions.PushBack(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	extensions.PushBack(VK_KHR_SURFACE_EXTENSION_NAME);
	extensions.PushBack("VK_KHR_win32_surface");

	Array<String> validationLayers;
	validationLayers.PushBack("VK_LAYER_LUNARG_standard_validation");

	VulkanContext* pContext = new VulkanContext("Sandbox", "Quartz Engine", extensions, validationLayers);
	VulkanSurface* pSurface = new VulkanSurface(pContext->GetInstance(), *pWindow, pContext->GetPrimaryDevice(), 640, 480, false, false);

	/* Start Engine */
	engine.Start(pWindow);
}