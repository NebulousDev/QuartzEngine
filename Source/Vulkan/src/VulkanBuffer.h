#pragma once

#include "Common.h"
#include "graphics\GFXBuffers.h"

#include "VulkanDevice.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class VulkanDeviceMemoryAllocation;

	class QUARTZ_API VulkanBuffer
	{
	protected:
		VulkanDevice*					mpParentDevice;
		VkBuffer						mBuffer;
		VkBufferUsageFlags				mUsage;
		UInt64							mSizeBytes;
		VulkanDeviceMemoryAllocation*	mpMemory;
		Bool8							mIsValidBuffer;

	public:
		VulkanBuffer() = default;
		VulkanBuffer(VulkanDevice& device, UInt64 sizeBytes, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties);

		VkBuffer GetBufferHandle() const { return mBuffer; }
		VkBufferUsageFlags GetBufferUsage() const { return mUsage; }
		VulkanDeviceMemoryAllocation* GetDeviceMemoryAllocation() { return mpMemory; }
		//UInt64 GetSizeBytes() const { return mSizeBytes; } //TODO: Ambiguous

		Bool8 IsValidBuffer() const { return mIsValidBuffer; }

		void Destroy();
	};

	class QUARTZ_API VulkanVertexBuffer : public GFXVertexBuffer, public VulkanBuffer
	{
	public:
		VulkanVertexBuffer(VulkanDevice& device, UInt64 sizeBytes, VkMemoryPropertyFlags memoryProperties, Bool8 hostVisible)
			: GFXVertexBuffer(sizeBytes, hostVisible),
			VulkanBuffer(device, sizeBytes, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, memoryProperties) {}
	};

	class QUARTZ_API VulkanIndexBuffer : public GFXIndexBuffer, public VulkanBuffer
	{
	public:
		VulkanIndexBuffer(VulkanDevice& device, UInt32 stride, UInt64 size, VkMemoryPropertyFlags memoryProperties, Bool8 hostVisible)
			: GFXIndexBuffer(stride, size, hostVisible),
			VulkanBuffer(device, stride * size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, memoryProperties) {}
	};

	class QUARTZ_API VulkanUniformBuffer : public GFXUniformBuffer, public VulkanBuffer
	{
	private:
		VkDescriptorSet mDescriptorSet;

	public:
		VulkanUniformBuffer(VulkanDevice& device, UInt32 sizeBytes, VkMemoryPropertyFlags memoryProperties, Bool8 hostVisible);
	
		VkDescriptorSet GetDescriptorSet() { return mDescriptorSet; }
	};

	class QUARTZ_API VulkanTextureBuffer : public GFXTextureBuffer, public VulkanBuffer
	{
	public:
		VulkanTextureBuffer(VulkanDevice& device, UInt64 sizeBytes, VkMemoryPropertyFlags memoryProperties, Bool8 hostVisible)
			: GFXTextureBuffer(sizeBytes, hostVisible),
			VulkanBuffer(device, sizeBytes, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, memoryProperties) {}
	};
}