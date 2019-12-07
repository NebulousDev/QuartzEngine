#include "VulkanTest.h"
#include <vulkan/vulkan.h>

namespace Quartz
{
	QUARTZ_API void TestVulkan()
	{
		Log.Info("Starting Vulkan Test...");

		VkInstance instance;

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Vulkan Test";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Quartz Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_1;

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

		Log.Info("Successfully created Vulkan instance!");
	}
}