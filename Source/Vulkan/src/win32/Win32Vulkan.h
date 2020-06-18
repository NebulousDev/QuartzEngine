#pragma once


#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN

#include <Windows.h>

#ifdef CreateWindow
#undef CreateWindow
#endif

#include "Common.h"
#include <vulkan\vulkan.h>

namespace Quartz
{
	Bool8 QUARTZ_API CreateWin32VulkanSurface(VkInstance instance, Handle64 window, VkSurfaceKHR* pSurface);
}