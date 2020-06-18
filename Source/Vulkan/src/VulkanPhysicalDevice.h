#pragma once

#include "gfx\PhysicalDevice.h"
#include "util/Array.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	struct QUARTZ_API VulkanPhysicalDevice : public PhysicalDevice
	{
	private:
		String mDeviceName;
		String mVendorName;

		Bool8 mIsDescreteGPU;
		Bool8 mIsIntegratedGPU;

		UInt32 mFamilyCount;

		VkPhysicalDevice					mPhysicalDevice;
		VkPhysicalDeviceProperties			mPhysicalDeviceProperties;
		VkPhysicalDeviceFeatures			mPhysicalDeviceFeatures;
		VkPhysicalDeviceMemoryProperties	mPhysicalDeviceMemoryProperties;

		Array<VkQueueFamilyProperties>		mQueueFamilyProperties;

	public:
		VulkanPhysicalDevice() = default;
		VulkanPhysicalDevice(VkPhysicalDevice physicalDevice);

		Int32 GetDeviceScore() const;

		const String& GetDeviceName() const { return mDeviceName; }
		const String& GetVendorName() const { return mVendorName; }

		Bool8 IsDiscreteGPU() const { return mIsDescreteGPU; }
		Bool8 IsIntegratedGPU() const { return mIsIntegratedGPU; }

		UInt32 GetFamilyCount() const { return mFamilyCount; }

		const VkPhysicalDevice&					GetPhysicalDeviceHandle() const { return mPhysicalDevice; }
		const VkPhysicalDeviceProperties&		GetPhysicalDeviceProperties() const { return mPhysicalDeviceProperties; }
		const VkPhysicalDeviceFeatures&			GetPhysicalDeviceFeatures() const { return mPhysicalDeviceFeatures; }
		const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties() const { return mPhysicalDeviceMemoryProperties; }
	
		const Array<VkQueueFamilyProperties>&	GetQueueFamilyProperties() const { return mQueueFamilyProperties; }
	};
}