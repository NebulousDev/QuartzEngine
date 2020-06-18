#pragma once

#include "gfx\GraphicsContext.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"

#include "util\Array.h"
#include "util\String.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanContext final : public GraphicsContext
	{
	private:
		String mAppName;
		String mEngineName;

		Bool8 mValidContext;

		VkInstance						mVkInstance;
		Array<VulkanPhysicalDevice>		mPhysicalDevices;
		VulkanDevice*					mpDevice;
		Array<VkLayerProperties>		mAvailableLayers;
		Array<VkExtensionProperties>	mAvailableExtensions;
		Array<VkPhysicalDevice>			mAvailablePhysicalDevices;
		Array<const char*>				mEnabledLayerNames;
		Array<const char*>				mEnabledExtensionNames;

		Bool8 InitInstance(const String& appName, const String& engineName,
			const Array<String>& extensions, const Array<String>& validationLayers);

		Bool8 InitDevices();

	public:
		VulkanContext(const String& appName, const String& engineName,
			const Array<String>& extensions, const Array<String>& validationLayers);

		const VkInstance& GetInstance() const { return mVkInstance; }
		const Array<VulkanPhysicalDevice>& GetPhysicalDevices() const { return mPhysicalDevices; }

		VulkanDevice& GetPrimaryDevice() { return *mpDevice; }

		const String& GetAppName() const { return mAppName; }
		const String& GetEngineName() const { return mEngineName; }

		Bool8 IsValidContext() const { return mValidContext; }
	};
}