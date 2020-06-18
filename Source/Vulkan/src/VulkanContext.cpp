#include "VulkanContext.h"

#include "log/log.h"

namespace Quartz
{
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT		messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT				messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void*										pUserData)
	{
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			Log.Error(pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
		{
			Log.Warning(pCallbackData->pMessage);
		}
		else
		{
			Log.Debug(pCallbackData->pMessage);
		}

		return VK_FALSE;
	}

	Bool8 EnumerateVkLayerProperties(Array<VkLayerProperties>& layerProperties)
	{
		VkResult result;
		UInt32 layerCount = 0;

		result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		layerProperties.Resize(layerCount);
		result = vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.Data());

		return result == VK_SUCCESS;
	}

	Bool8 EnumerateVkExtensionProperties(Array<VkExtensionProperties>& extensionProperties)
	{
		VkResult result;
		UInt32 extensionCount = 0;

		result = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
		extensionProperties.Resize(extensionCount);
		result = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionProperties.Data());
	
		return result == VK_SUCCESS;
	}

	Bool8 EnumerateVkPhysicalDevices(VkInstance instance, Array<VkPhysicalDevice>& physicalDevices)
	{
		VkResult result;
		UInt32 physicalDeviceCount;

		result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
		physicalDevices.Resize(physicalDeviceCount);
		result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.Data());

		return result == VK_SUCCESS;
	}

	Bool8 VulkanContext::InitInstance(const String& appName, const String& engineName,
		const Array<String>& extensions, const Array<String>& validationLayers)
	{
		if (!EnumerateVkLayerProperties(mAvailableLayers))
		{
			Log.Warning("Failed to create a vulkan instance: Unable to enumerate validation layers!");
		}

		for (const String& layerName : validationLayers)
		{
			for (const VkLayerProperties& layerProp : mAvailableLayers)
			{
				if (layerName == layerProp.layerName)
				{
					mEnabledLayerNames.PushBack(layerName.Str());
					goto layerFound;
				}
			}

			Log.Warning("Attempted to enable unsupported vulkan instance layer [\'%s\']!", layerName.Str());

			layerFound:;
		}

		if (!EnumerateVkExtensionProperties(mAvailableExtensions))
		{
			Log.Warning("Failed to create a vulkan instance: Unable to enumerate extensions!");
		}

		for (const String& extName : extensions)
		{
			for (const VkExtensionProperties& extProp : mAvailableExtensions)
			{
				if (extName == extProp.extensionName)
				{
					mEnabledExtensionNames.PushBack(extName.Str());
					goto extFound;
				}
			}

			Log.Warning("Attempted to enable unsupported vulkan instance extension [\'%s\']!", extName.Str());

			extFound:;
		}

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
		debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerInfo.pfnUserCallback = DebugCallback;
		debugMessengerInfo.pUserData = NULL;

		VkApplicationInfo appInfo	= {};
		appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName	= appName.Str();
		appInfo.applicationVersion	= VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName			= engineName.Str();
		appInfo.engineVersion		= VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion			= VK_API_VERSION_1_1;

		VkInstanceCreateInfo createInfo		= {};
		createInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo			= &appInfo;
		createInfo.enabledLayerCount		= mEnabledLayerNames.Size();
		createInfo.ppEnabledLayerNames		= mEnabledLayerNames.Data();
		createInfo.enabledExtensionCount	= mEnabledExtensionNames.Size();
		createInfo.ppEnabledExtensionNames	= mEnabledExtensionNames.Data();
		createInfo.pNext					= (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;

		if (vkCreateInstance(&createInfo, NULL, &mVkInstance) != VK_SUCCESS)
		{
			Log.Critical("Failed to create a vulkan instance: vkCreateInstance failed!");
			return false;
		}

		return true;
	}

	Bool8 VulkanContext::InitDevices()
	{
		if (!EnumerateVkPhysicalDevices(mVkInstance, mAvailablePhysicalDevices))
		{
			Log.Error("Failed to initialize graphics device: vkEnumeratePhysicalDevices failed!");
			return false;
		}

		mPhysicalDevices.Resize(mAvailablePhysicalDevices.Size());

		Int32 maxDeviceScore = -1;
		VulkanPhysicalDevice* pPhyiscalDeviceCandidate = nullptr;

		Int32 deviceIndex = 0;
		for (const VkPhysicalDevice physicalDevice : mAvailablePhysicalDevices)
		{
			mPhysicalDevices[deviceIndex] = VulkanPhysicalDevice(physicalDevice);

			Int32 deviceScore = mPhysicalDevices[deviceIndex].GetDeviceScore();

			if (deviceScore > maxDeviceScore)
			{
				maxDeviceScore = deviceScore;
				pPhyiscalDeviceCandidate = &mPhysicalDevices[deviceIndex];
			}

			++deviceIndex;
		}

		if (pPhyiscalDeviceCandidate == nullptr)
		{
			Log.Critical("Failed to initialize graphics device: No suitable graphics adapter was detected!");
			return false;
		}

		Array<String> deviceExtensions;
		deviceExtensions.PushBack(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		deviceExtensions.PushBack(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
		deviceExtensions.PushBack("VK_EXT_full_screen_exclusive");

		mpDevice = new VulkanDevice(&mPhysicalDevices[deviceIndex - 1], deviceExtensions);

		if (!mpDevice->IsValidDevice())
		{
			Log.Critical("Failed to initialize graphics device: No valid device was created!");
			return false;
		}

		return true;
	}

	VulkanContext::VulkanContext(const String& appName, const String& engineName,
		const Array<String>& extensions, const Array<String>& validationLayers)
	{
		mAppName = appName;
		mEngineName = engineName;

		if (
			InitInstance(appName, engineName, extensions, validationLayers) &&
			InitDevices()
			)
		{
			mValidContext = true;
		}
	}
}

