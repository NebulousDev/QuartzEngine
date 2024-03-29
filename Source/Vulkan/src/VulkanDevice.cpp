#include "VulkanDevice.h"
#include "VulkanDeviceMemory.h"

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
		if (!EnumerateDeviceExtensionProperties(pPhysicalDevice->GetPhysicalDeviceHandle(), mAvailableExtensionProperties))
		{
			Log::Error("Failed to create vulkan logical device: Unable to enumerate device extesnsion properties!");
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

			Log::Warning("Attempted to enable unsupported device extension [\'%s\']!", extName.Str());
			
			extFound:;
		}

		Array<Array<VkExtensionProperties>> availableLayerExtensionPropertiesList;
		availableLayerExtensionPropertiesList.Resize(mAvailableLayerProperties.Size());


		// NOTE: According to the vulkan spec, device-specific layers are now deprecated.
		// Some code only acts as a fallback for users without updated drivers.

		// NOTE: I Also need to keep layer extensions available for RenderDoc

		/*
		if (!EnumerateDeviceLayerProperties(pPhysicalDevice->GetPhysicalDeviceHandle(), mAvailableLayerProperties))
		{
			Log::Error("Failed to create vulkan logical device: Unable to enumerate device layer properties!");
			return false;
		}

		UInt32 layerPropertiesIndex = 0;
		for (const VkLayerProperties& layer : mAvailableLayerProperties)
		{
			Array<VkExtensionProperties>& availableLayerExtensionProperties = availableLayerExtensionPropertiesList[layerPropertiesIndex];

			if (!EnumerateDeviceLayerExtentionProperties(pPhysicalDevice->GetPhysicalDeviceHandle(), layer, availableLayerExtensionProperties))
			{
				Log::Warning("Failed to create vulkan logical device: Unable to enumerate device layer extensions for layer \'%s\'!", layer.layerName);
				//return false;
			}

			mEnabledValidationLayerNames.PushBack(layer.layerName);

			for (const VkExtensionProperties& extension : availableLayerExtensionProperties)
			{
				if (!mEnabledExtensionNames.Contains(extension.extensionName))
				{
					mEnabledExtensionNames.PushBack(extension.extensionName);
				}
			}

			++layerPropertiesIndex;
		}
		*/

		Array<VkQueueFamilyProperties> queueFamilyProperties;

		if (!EnumerateDeviceQueueFamilyProperties(pPhysicalDevice->GetPhysicalDeviceHandle(), queueFamilyProperties))
		{
			Log::Error("Failed to create vulkan logical device: Unable to enumerate device queue family properties!");
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
			Log::Critical("Failed to create logical device: No suitable graphics queue family found!");
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
		deviceFeatures.samplerAnisotropy = VK_TRUE;	//TODO: expose

		VkDeviceCreateInfo deviceInfo		= {};
		deviceInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.queueCreateInfoCount		= queueCreateInfos.Size();
		deviceInfo.pQueueCreateInfos		= queueCreateInfos.Data();
		deviceInfo.enabledLayerCount		= mEnabledValidationLayerNames.Size();
		deviceInfo.ppEnabledLayerNames		= mEnabledValidationLayerNames.Data();
		deviceInfo.enabledExtensionCount	= mEnabledExtensionNames.Size();
		deviceInfo.ppEnabledExtensionNames	= mEnabledExtensionNames.Data();
		deviceInfo.pEnabledFeatures			= &deviceFeatures;

		if (vkCreateDevice(pPhysicalDevice->GetPhysicalDeviceHandle(), &deviceInfo, nullptr, &mDevice) != VK_SUCCESS)
		{
			Log::Critical("Failed to create logical device: vkCreateDevice failed!");
			return false;
		}

		mpGraphicsQueue = new VulkanQueue(*this, graphicsQueueIndex, queueFamilyProperties[graphicsQueueIndex]);
		mpTransferQueue = mpGraphicsQueue;
		mpComputeQueue = mpGraphicsQueue;

		if (transferQueueIndex != -1)
		{
			mpTransferQueue = new VulkanQueue(*this, transferQueueIndex, queueFamilyProperties[transferQueueIndex]);
		}

		if (computeQueueIndex != -1)
		{
			mpComputeQueue = new VulkanQueue(*this, computeQueueIndex, queueFamilyProperties[computeQueueIndex]);
		}

		// TODO: allow better present queue selection
		mpPresentQueue = mpGraphicsQueue;

		vkSetDebugUtilsObjectTag = (PFN_vkSetDebugUtilsObjectTagEXT)vkGetDeviceProcAddr(mDevice, "vkSetDebugUtilsObjectTagEXT");
		vkSetDebugUtilsObjectName = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetDeviceProcAddr(mDevice, "vkSetDebugUtilsObjectNameEXT");
		vkCmdBeginDebugUtilsLabel = (PFN_vkCmdBeginDebugUtilsLabelEXT)vkGetDeviceProcAddr(mDevice, "vkCmdBeginDebugUtilsLabelEXT");
		vkCmdInsertDebugUtilsLabel = (PFN_vkCmdInsertDebugUtilsLabelEXT)vkGetDeviceProcAddr(mDevice, "vkCmdInsertDebugUtilsLabelEXT");
		vkCmdEndDebugUtilsLabel = (PFN_vkCmdEndDebugUtilsLabelEXT)vkGetDeviceProcAddr(mDevice, "vkCmdEndDebugUtilsLabelEXT");

		return true;
	}

#define MAX_DESCRIPTOR_POOL_SETS		16
#define MAX_UNIFORM_DESCRIPTOR_COUNT	16

	// TODO: move this out of device
	Bool8 VulkanDevice::InitPools()
	{
		VkDescriptorPoolSize uniformDescriptorPoolSize = {};
		uniformDescriptorPoolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		uniformDescriptorPoolSize.descriptorCount = MAX_UNIFORM_DESCRIPTOR_COUNT;

		VkDescriptorPoolSize poolSizes[] = { uniformDescriptorPoolSize };

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = 1;
		poolInfo.pPoolSizes = poolSizes;
		poolInfo.maxSets = MAX_DESCRIPTOR_POOL_SETS;
		poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT;

		if (vkCreateDescriptorPool(mDevice, &poolInfo, nullptr, &mDescriptorPool) != VK_SUCCESS)
		{
			Log::Error("Failed to create descriptor pool: vkCreateDescriptorPool failed!");
			return false;
		}

		VkCommandPoolCreateInfo graphicsCommandPoolInfo = {};
		graphicsCommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		graphicsCommandPoolInfo.queueFamilyIndex = mpGraphicsQueue->GetFamilyIndex();
		graphicsCommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

		if (vkCreateCommandPool(mDevice, &graphicsCommandPoolInfo, nullptr, &mGraphicsCommandPool) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan graphics command pool: vkCreateCommandPool failed!");
			return false;
		}

		if (mpComputeQueue != mpGraphicsQueue)
		{
			VkCommandPoolCreateInfo computeCommandPoolInfo = {};
			computeCommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			computeCommandPoolInfo.queueFamilyIndex = mpComputeQueue->GetFamilyIndex();
			computeCommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

			if (vkCreateCommandPool(mDevice, &computeCommandPoolInfo, nullptr, &mComputeCommandPool) != VK_SUCCESS)
			{
				Log::Error("Failed to create vulkan compute command pool: vkCreateCommandPool failed!");
				return false;
			}
		}
		else
		{
			mComputeCommandPool = mGraphicsCommandPool;
		}

		if (mpTransferQueue != mpGraphicsQueue)
		{
			VkCommandPoolCreateInfo transferCommandPoolInfo = {};
			transferCommandPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			transferCommandPoolInfo.queueFamilyIndex = mpTransferQueue->GetFamilyIndex();
			transferCommandPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

			if (vkCreateCommandPool(mDevice, &transferCommandPoolInfo, nullptr, &mTransferCommandPool) != VK_SUCCESS)
			{
				Log::Error("Failed to create vulkan transfer command pool: vkCreateCommandPool failed!");
				return false;
			}
		}
		else
		{
			mTransferCommandPool = mGraphicsCommandPool;
		}

		return true;
	}

	VulkanDevice::VulkanDevice(VulkanPhysicalDevice* pPhysicalDevice, const Array<String>& deviceExtensions)
		: mDeviceMemoryAllocator(*this)
	{
		mpPhysicalDevice = pPhysicalDevice;

		if (
			InitDevice(pPhysicalDevice, deviceExtensions) &&
			InitPools()
			)
		{
			mIsValidDevice = true;
		}

		// TODO: Compare with hash set
		for (const char* extName : mEnabledExtensionNames)
		{
			if (StringCompare(extName, VK_EXT_DEBUG_UTILS_EXTENSION_NAME) == 0)
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

	void VulkanDevice::SetDebugName(Handle64 object, VkObjectType type, const String& debugName)
	{
		VkDebugUtilsObjectNameInfoEXT nameInfo = {};
		nameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT;
		nameInfo.objectType = type;
		nameInfo.objectHandle = static_cast<UInt64>(object);
		nameInfo.pObjectName = debugName.Str();

		vkSetDebugUtilsObjectName(mDevice, &nameInfo);
	}
}

