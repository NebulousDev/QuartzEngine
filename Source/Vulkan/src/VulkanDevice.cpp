#include "VulkanDevice.h"

#include "log/log.h"

namespace Quartz
{
	Bool8 EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, Array<VkLayerProperties>& layerProperties)
	{
		VkResult result;
		UInt32 layerCount = 0;

		result = vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, nullptr);
		layerProperties.Resize(layerCount);
		result = vkEnumerateDeviceLayerProperties(physicalDevice, &layerCount, layerProperties.Data());

		return result == VK_SUCCESS;
	}

	Bool8 EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, Array<VkExtensionProperties>& extensionProperties)
	{
		VkResult result;
		UInt32 extCount = 0;

		result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
		extensionProperties.Resize(extCount);
		result = vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, extensionProperties.Data());

		return result == VK_SUCCESS;
	}

	Bool8 EnumerateDeviceLayerExtentionProperties(VkPhysicalDevice physicalDevice, VkLayerProperties layerProperties, Array<VkExtensionProperties>& extensionProperties)
	{
		VkResult result;
		UInt32 extCount = 0;

		result = vkEnumerateDeviceExtensionProperties(physicalDevice, layerProperties.layerName, &extCount, nullptr);
		extensionProperties.Resize(extCount);
		result = vkEnumerateDeviceExtensionProperties(physicalDevice, layerProperties.layerName, &extCount, extensionProperties.Data());

		return result == VK_SUCCESS;
	}

	Bool8 EnumerateDeviceQueueFamilyProperties(VkPhysicalDevice physicalDevice, Array<VkQueueFamilyProperties>& queueFamilyProperties)
	{
		UInt32 familyCount = 0;

		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, nullptr);
		queueFamilyProperties.Resize(familyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &familyCount, queueFamilyProperties.Data());

		return true;
	}

	Bool8 VulkanDevice::InitDevice(VulkanPhysicalDevice* pPhysicalDevice, const Array<String>& deviceExtensions)
	{
		// NOTE: According to the vulkan spec, device-specific layers are now deprecated.
		// Some code only acts as a fallback for users without updated drivers.

		if (!EnumerateDeviceLayerProperties(pPhysicalDevice->GetPhysicalDeviceHandle(), mAvailableLayerProperties))
		{
			Log.Error("Failed to create vulkan logical device: Unable to enumerate device layer properties!");
			return false;
		}

		if (!EnumerateDeviceExtensionProperties(pPhysicalDevice->GetPhysicalDeviceHandle(), mAvailableExtensionProperties))
		{
			Log.Error("Failed to create vulkan logical device: Unable to enumerate device extesnsion properties!");
			return false;
		}

		for (const String& extName : deviceExtensions)
		{
			for (const VkExtensionProperties& extensionProperty : mAvailableExtensionProperties)
			{
				if (extName == extensionProperty.extensionName)
				{
					mEnabledExtensionNames.PushBack(extName.Str());
					goto extFound;
				}

			}

			Log.Warning("Attempted to enable unsupported device extension [\'%s\']!", extName.Str());
			
			extFound:;
		}

		for (const VkLayerProperties& layer : mAvailableLayerProperties)
		{
			Array<VkExtensionProperties> availableLayerExtensionProperties;

			if (!EnumerateDeviceLayerExtentionProperties(pPhysicalDevice->GetPhysicalDeviceHandle(), layer, availableLayerExtensionProperties))
			{
				Log.Warning("Failed to create vulkan logical device: Unable to enumerate device layer extensions for layer \'%s\'!", layer.layerName);
				//return false;
			}

			mEnabledLayerNames.PushBack(layer.layerName);

			for (const VkExtensionProperties& extension : availableLayerExtensionProperties)
			{
				if (!mEnabledExtensionNames.Contains(extension.extensionName))
				{
					mEnabledExtensionNames.PushBack(extension.extensionName);
				}
			}
		}

		Array<VkQueueFamilyProperties> queueFamilyProperties;

		if (!EnumerateDeviceQueueFamilyProperties(pPhysicalDevice->GetPhysicalDeviceHandle(), queueFamilyProperties))
		{
			Log.Error("Failed to create vulkan logical device: Unable to enumerate device queue family properties!");
			return false;
		}

		UInt32 graphicsQueueIndex = -1;
		UInt32 transferQueueIndex = -1;
		UInt32 computeQueueIndex  = -1;

		UInt32 familyIndex = 0;
		for (const VkQueueFamilyProperties& queuefamilyProperty : queueFamilyProperties)
		{
			if ((queuefamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) == VK_QUEUE_GRAPHICS_BIT)
			{
				if (graphicsQueueIndex == -1)
				{
					graphicsQueueIndex = familyIndex;
				}
			}

			if ((queuefamilyProperty.queueFlags & VK_QUEUE_TRANSFER_BIT) == VK_QUEUE_TRANSFER_BIT)
			{
				if (transferQueueIndex == -1 && 
					(graphicsQueueIndex != familyIndex) &&
					(queuefamilyProperty.queueFlags & VK_QUEUE_GRAPHICS_BIT) != VK_QUEUE_GRAPHICS_BIT &&
					(queuefamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) != VK_QUEUE_COMPUTE_BIT)
				{
					transferQueueIndex = familyIndex;
				}
			}

			if ((queuefamilyProperty.queueFlags & VK_QUEUE_COMPUTE_BIT) == VK_QUEUE_COMPUTE_BIT)
			{
				if (computeQueueIndex == -1 && (graphicsQueueIndex != familyIndex))
				{
					computeQueueIndex = familyIndex;
				}
			}

			++familyIndex;
		}

		Array<VkDeviceQueueCreateInfo> queueCreateInfos;

		Array<Float32> queuePriorites(1);
		queuePriorites[0] = 1.0f;

		if (graphicsQueueIndex != -1)
		{
			VkDeviceQueueCreateInfo graphicsQueueInfo = {};
			graphicsQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			graphicsQueueInfo.queueFamilyIndex = graphicsQueueIndex;
			graphicsQueueInfo.queueCount = 1;
			graphicsQueueInfo.pQueuePriorities = queuePriorites.Data();

			queueCreateInfos.PushBack(graphicsQueueInfo);
		}
		else
		{
			Log.Critical("Failed to create logical device: No suitable graphics queue family found!");
			return false;
		}

		if (transferQueueIndex != -1)
		{
			VkDeviceQueueCreateInfo transferQueueInfo	= {};
			transferQueueInfo.sType						= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			transferQueueInfo.queueFamilyIndex			= transferQueueIndex;
			transferQueueInfo.queueCount				= 1;
			transferQueueInfo.pQueuePriorities			= queuePriorites.Data();

			queueCreateInfos.PushBack(transferQueueInfo);
		}

		if (computeQueueIndex != -1)
		{
			// TODO: check compute compatability?
			VkDeviceQueueCreateInfo computeQueueInfo	= {};
			computeQueueInfo.sType						= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			computeQueueInfo.queueFamilyIndex			= computeQueueIndex;
			computeQueueInfo.queueCount					= 1;
			computeQueueInfo.pQueuePriorities			= queuePriorites.Data();

			queueCreateInfos.PushBack(computeQueueInfo);
		}

		VkPhysicalDeviceFeatures deviceFeatures {};

		VkDeviceCreateInfo deviceInfo		= {};
		deviceInfo.queueCreateInfoCount		= queueCreateInfos.Size();
		deviceInfo.pQueueCreateInfos		= queueCreateInfos.Data();
		deviceInfo.enabledLayerCount		= mEnabledLayerNames.Size();
		deviceInfo.ppEnabledLayerNames		= mEnabledLayerNames.Data();
		deviceInfo.enabledExtensionCount	= mEnabledExtensionNames.Size();
		deviceInfo.ppEnabledExtensionNames	= mEnabledExtensionNames.Data();
		deviceInfo.pEnabledFeatures			= &deviceFeatures;

		if (vkCreateDevice(pPhysicalDevice->GetPhysicalDeviceHandle(), &deviceInfo, nullptr, &mDevice) != VK_SUCCESS)
		{
			Log.Critical("Failed to create logical device: vkCreateDevice failed!");
			return false;
		}

		mpGraphicsQueue = new VulkanQueue(this, graphicsQueueIndex);
		mpTransferQueue = mpGraphicsQueue;
		mpComputeQueue = mpGraphicsQueue;

		if (transferQueueIndex != -1)
		{
			mpTransferQueue = new VulkanQueue(this, transferQueueIndex);
		}

		if (computeQueueIndex != -1)
		{
			mpComputeQueue = new VulkanQueue(this, computeQueueIndex);
		}

		// TODO: allow better present queue selection
		mpPresentQueue = mpGraphicsQueue;

		return true;
	}

	VulkanDevice::VulkanDevice(VulkanPhysicalDevice* pPhysicalDevice, const Array<String>& deviceExtensions)
	{
		mpPhysicalDevice = pPhysicalDevice;

		if (InitDevice(pPhysicalDevice, deviceExtensions))
		{
			mIsValidDevice = true;
		}

		for (const char* extName : mEnabledExtensionNames)
		{
			if (StringCompare(extName, VK_EXT_DEBUG_MARKER_EXTENSION_NAME) == 0)
			{
				mSupportsDebugNames = true;
			}

			else if (StringCompare(extName, "VK_EXT_full_screen_exclusive") == 0)
			{
				mSupportsFullscreen = true;
			}
		}
	}

	void VulkanDevice::DestroyDevice()
	{
		if (mpTransferQueue != mpGraphicsQueue)
		{
			delete mpTransferQueue;
		}

		if (mpComputeQueue != mpGraphicsQueue)
		{
			delete mpComputeQueue;
		}

		delete mpGraphicsQueue;

		vkDestroyDevice(mDevice, nullptr);
	}
}

