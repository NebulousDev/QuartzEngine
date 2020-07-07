#include "VulkanBuffer.h"
#include "VulkanDeviceMemory.h"

#include "VulkanPipeline.h"

#include "Log/log.h"

namespace Quartz
{
	VulkanBuffer::VulkanBuffer(VulkanDevice& device, UInt64 sizeBytes, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties)
	{
		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeBytes;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(device.GetDeviceHandle(), &bufferInfo, nullptr, &mBuffer) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan buffer object: vkCreateBuffer failed!");
			return;
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(device.GetDeviceHandle(), mBuffer, &memRequirements);

		VulkanDeviceMemoryAllocator& allocator = device.GetDeviceMemoryAllocator();
		mpMemory = allocator.Allocate(memRequirements.size, memRequirements.memoryTypeBits, memoryProperties);

		if (mpMemory == nullptr)
		{
			Log.Error("Failed to create vulkan buffer object: Failed to allocate device memory!");
			return;
		}

		if (vkBindBufferMemory(device.GetDeviceHandle(), mBuffer, mpMemory->GetDeviceMemoryHandle(), 0) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan buffer object: vkBindBufferMemory failed!");

			allocator.Free(mpMemory);
			vkDestroyBuffer(device.GetDeviceHandle(), mBuffer, nullptr);

			return;
		}

		mpParentDevice = &device;
		mUsage = usage;
		mSizeBytes = sizeBytes;
		mIsValidBuffer = true;
	}

	void VulkanBuffer::Destroy()
	{
		if (mIsValidBuffer)
		{
			vkDestroyBuffer(mpParentDevice->GetDeviceHandle(), mBuffer, nullptr);

			// Free descriptor set
		}
	}

	VulkanUniformBuffer::VulkanUniformBuffer(VulkanDevice& device, UInt32 sizeBytes, VkMemoryPropertyFlags memoryProperties, Bool8 hostVisible)
		: GFXUniformBuffer(sizeBytes, hostVisible),
		VulkanBuffer(device, sizeBytes, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, memoryProperties)
	{
		///////////////////////////////////////////////////////////////////////////////////

		VulkanDescriptorSetLayoutBinding binding;
		binding.binding = 0;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		Array<VulkanDescriptorSetLayoutBinding> descriptorBindings;
		descriptorBindings.PushBack(binding);
		VulkanDescriptorSetLayout descriptorLayout(device, descriptorBindings);

		VkDescriptorSetLayout setLayout = descriptorLayout.GetDescriptorSetLayoutHandle();
		VkDescriptorSetLayout setLayouts[] = { setLayout };

		///////////////////////////////////////////////////////////////////////////////////

		VkDescriptorSetAllocateInfo descriptorAllocInfo = {};
		descriptorAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		descriptorAllocInfo.descriptorPool = device.GetDescriptorPool();
		descriptorAllocInfo.descriptorSetCount = 1;
		descriptorAllocInfo.pSetLayouts = setLayouts;

		if (vkAllocateDescriptorSets(device.GetDeviceHandle(), &descriptorAllocInfo, &mDescriptorSet) != VK_SUCCESS)
		{
			Log.Error("Failed to allocate descriptor set: vkAllocateDescriptorSets failed!");
		}

		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = mBuffer;
		bufferInfo.offset = 0;
		bufferInfo.range = VK_WHOLE_SIZE;

		VkWriteDescriptorSet writeDescriptor = {};
		writeDescriptor.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		writeDescriptor.dstSet = mDescriptorSet;
		writeDescriptor.dstBinding = 0;
		writeDescriptor.dstArrayElement = 0;
		writeDescriptor.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		writeDescriptor.descriptorCount = 1;
		writeDescriptor.pBufferInfo = &bufferInfo;
		writeDescriptor.pImageInfo = nullptr;
		writeDescriptor.pTexelBufferView = nullptr;

		vkUpdateDescriptorSets(device.GetDeviceHandle(), 1, &writeDescriptor, 0, nullptr);
	}
}

