#pragma once

#include "graphics\GFXDevice.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanQueue.h"
#include "VulkanDeviceMemory.h"

#include "util/String.h"
#include "util/Array.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanDevice final : public GFXDevice
	{
	private:
		VkDevice				mDevice;
		VulkanPhysicalDevice*	mpPhysicalDevice;

		Array<VkLayerProperties>		mAvailableLayerProperties;
		Array<VkExtensionProperties>	mAvailableExtensionProperties;
		Array<const char*>				mEnabledLayerNames;
		Array<const char*>				mEnabledExtensionNames;

		VulkanDeviceMemoryAllocator		mDeviceMemoryAllocator;
		VkDescriptorPool				mDescriptorPool;
		VkCommandPool					mGraphicsCommandPool;
		VkCommandPool					mComputeCommandPool;
		VkCommandPool					mTransferCommandPool;

		VulkanQueue* mpGraphicsQueue;
		VulkanQueue* mpComputeQueue;
		VulkanQueue* mpTransferQueue;
		VulkanQueue* mpPresentQueue;

		PFN_vkDebugMarkerSetObjectTagEXT	vkDebugMarkerSetObjectTag;
		PFN_vkDebugMarkerSetObjectNameEXT	vkDebugMarkerSetObjectName;
		PFN_vkCmdDebugMarkerBeginEXT		vkCmdDebugMarkerBegin;
		PFN_vkCmdDebugMarkerEndEXT			vkCmdDebugMarkerEnd;
		PFN_vkCmdDebugMarkerInsertEXT		vkCmdDebugMarkerInsert;

		struct
		{
			Bool8 mSupportsDebugNames	: 1;
			Bool8 mSupportsFullscreen	: 1;
			Bool8 mIsValidDevice		: 1;
		};


	private:
		Bool8 InitDevice(VulkanPhysicalDevice* pPhysicalDevice, const Array<String>& deviceExtensions);
		Bool8 InitPools();

	public:
		VulkanDevice(VulkanPhysicalDevice* pPhysicalDevice, const Array<String>& deviceExtensions);

		void DestroyDevice();

		void SetDebugMarkerObjectName(Handle64 object, VkDebugReportObjectTypeEXT type, const String& debugName);

		const VkDevice& GetDeviceHandle() const { return mDevice; }
		const VulkanPhysicalDevice& GetPhysicalDevice() { return *mpPhysicalDevice; }

		VulkanDeviceMemoryAllocator& GetDeviceMemoryAllocator() { return mDeviceMemoryAllocator; }
		VkDescriptorPool& GetDescriptorPool() { return mDescriptorPool; }

		VulkanQueue& GetGrahpicsQueue() { return *mpGraphicsQueue; }
		VulkanQueue& GetTransferQueue() { return *mpTransferQueue; }
		VulkanQueue& GetComputeQueue() { return *mpComputeQueue; }
		VulkanQueue& GetPresentQueue() { return *mpPresentQueue; }

		VkCommandPool GetGraphicsCommandPoolHandle() const { return mGraphicsCommandPool; }
		VkCommandPool GetComputeCommandPoolHandle() const { return mComputeCommandPool; }
		VkCommandPool GetTransferCommandPoolHandle() const { return mTransferCommandPool; }

		Bool8 HasUniqueTransferQueue() const { return mpTransferQueue != mpGraphicsQueue; }
		Bool8 HasUniqueComputeQueue() const { return mpComputeQueue != mpGraphicsQueue; }

		Bool8 SupportsDebugNames() const { return mSupportsDebugNames; }
		Bool8 SupportsFullscreen() const { return mSupportsFullscreen; }

		Bool8 IsValidDevice() const { return mIsValidDevice; }
	};
}