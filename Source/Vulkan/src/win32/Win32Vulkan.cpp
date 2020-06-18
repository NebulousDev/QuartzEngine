#include "Win32Vulkan.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <Windows.h>

#ifdef CreateWindow
#undef CreateWindow
#endif

#include <vulkan\vulkan_win32.h>

namespace Quartz
{
	Bool8 CreateWin32VulkanSurface(VkInstance instance, Handle64 window, VkSurfaceKHR* pSurface)
	{
		VkWin32SurfaceCreateInfoKHR win32SurfaceInfo = {};
		win32SurfaceInfo.sType		= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		win32SurfaceInfo.hinstance	= GetModuleHandle(nullptr);
		win32SurfaceInfo.hwnd		= (HWND)window;

		VkResult result = vkCreateWin32SurfaceKHR(instance, &win32SurfaceInfo, nullptr, pSurface);

		return result == VK_SUCCESS;
	}
}

