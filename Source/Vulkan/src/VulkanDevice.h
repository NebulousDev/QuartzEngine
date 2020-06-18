#pragma once

#include "gfx\GraphicsDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanQueue.h"

#include "util/String.h"
#include "util/Array.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanDevice final : public GraphicsDevice
	{
	private:
		VkDevice				mDevice;
		VulkanPhysicalDevice*	mpPhysicalDevice;

		Array<VkLayerProperties>		mAvailableLayerProperties;
		Array<VkExtensionProperties>	mAvailableExtensionProperties;
		Array<const char*>				mEnabledLayerNames;
		Array<const char*>				mEnabledExtensionNames;

		VulkanQueue* mpGraphicsQueue;
		VulkanQueue* mpTransferQueue;
		VulkanQueue* mpComputeQueue;
		VulkanQueue* mpPresentQueue;

		struct
		{
			Bool8 mSupportsDebugNames	: 1;
			Bool8 mSupportsFullscreen	: 1;
			Bool8 mIsValidDevice		: 1;
		};


	private:
		Bool8 InitDevice(VulkanPhysicalDevice* pPhysicalDevice, const Array<String>& deviceExtensions);

	public:
		VulkanDevice(VulkanPhysicalDevice* pPhysicalDevice, const Array<String>& deviceExtensions);

		void DestroyDevice();

		const VkDevice& GetDeviceHandle() const { return mDevice; }
		const VulkanPhysicalDevice& GetPhysicalDeviceHandle() { return *mpPhysicalDevice; }

		VulkanQueue& GetGrahpicsQueue() { return *mpGraphicsQueue; }
		VulkanQueue& GetTransferQueue() { return *mpTransferQueue; }
		VulkanQueue& GetComputeQueue() { return *mpComputeQueue; }
		VulkanQueue& GetPresentQueue() { return *mpPresentQueue; }

		Bool8 HasUniqueTransferQueue() const { return mpTransferQueue != mpGraphicsQueue; }
		Bool8 HasUniqueComputeQueue() const { return mpComputeQueue != mpGraphicsQueue; }

		Bool8 SupportsDebugNames() const { return mSupportsDebugNames; }
		Bool8 SupportsFullscreen() const { return mSupportsFullscreen; }

		Bool8 IsValidDevice() const { return mIsValidDevice; }
	};
}