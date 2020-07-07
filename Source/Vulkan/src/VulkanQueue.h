#pragma once

#include "Common.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class VulkanDevice;

	class QUARTZ_API VulkanQueue
	{
	private:
		VkQueue			mQueue;
		VulkanDevice*	mpParentDevice;
		UInt32			mFamilyIndex;
		UInt32			mQueueIndex;
		
		struct
		{
			Bool8 mSupportsGraphics : 1;
			Bool8 mSupportsTransfer : 1;
			Bool8 mSupportsCompute  : 1;
			Bool8 mSupportsPresent  : 1;
		};

	public:
		VulkanQueue(VulkanDevice& device, UInt32 familyIndex, VkQueueFamilyProperties queueFamilyProperties);

		const VkQueue& GetQueueHandle() const { return mQueue; }
		const VulkanDevice* GetParentDevice() const { return mpParentDevice; }
		
		UInt32 GetFamilyIndex() const { return mFamilyIndex; }
		UInt32 GetQueueIndex() const { return mQueueIndex; }

		Bool8 SupportsGraphics() const { return mSupportsGraphics; }
		Bool8 SupportsTransfer() const { return mSupportsTransfer; }
		Bool8 SupportsCompute() const { return mSupportsCompute; }
		Bool8 SupportsPresent() const { return mSupportsTransfer; } // TODO: look into this
	};
}