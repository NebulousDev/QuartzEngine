#include "VulkanPhysicalDevice.h"

#include "gfx/GraphicsUtil.h"

namespace Quartz
{
	VulkanPhysicalDevice::VulkanPhysicalDevice(VkPhysicalDevice physicalDevice)
	{
		mPhysicalDevice = physicalDevice;
		vkGetPhysicalDeviceProperties(physicalDevice, &mPhysicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(physicalDevice, &mPhysicalDeviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(physicalDevice, &mPhysicalDeviceMemoryProperties);

		mDeviceName = mPhysicalDeviceProperties.deviceName;
		mVendorName = DeviceVendorNameById(mPhysicalDeviceProperties.vendorID);

		mIsDescreteGPU = mPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;
		mIsIntegratedGPU = mPhysicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
	
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &mFamilyCount, nullptr);
		mQueueFamilyProperties.Resize(mFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &mFamilyCount, mQueueFamilyProperties.Data());
	}

	Int32 VulkanPhysicalDevice::GetDeviceScore() const
	{
		Int32 deviceScore = 0;

		if (mIsDescreteGPU)
		{
			deviceScore += 1000;
		}

		deviceScore += mPhysicalDeviceProperties.limits.maxImageDimension2D;
		deviceScore += (mFamilyCount * 10);

		// TODO: geom/tess shader check?

		return deviceScore;
	}

}

