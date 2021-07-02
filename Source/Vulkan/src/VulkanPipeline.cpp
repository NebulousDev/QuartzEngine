#include "VulkanPipeline.h"

#include "Engine.h"

namespace Quartz
{
	template<>
	UInt32 Hash<UniformSetBinding>(const UniformSetBinding& value)
	{
		return Hash<UInt32>(value.intValue);
	}

	Bool8 UniformSetBinding::operator==(const UniformSetBinding& other)
	{
		return intValue == other.intValue;
	}

	VulkanGraphicsPipeline::VulkanGraphicsPipeline(VkPipeline vkPipeline, VulkanDevice* pDevice, const Array<VulkanDescriptorSetInfo>& descriptorSetInfos,
		VkGraphicsPipelineCreateInfo vkPipelineInfo, const GraphicsPipelineInfo& info)
		:mvkPipeline(vkPipeline),
		mpDevice(pDevice),
		mvkPipelineInfo(vkPipelineInfo),
		mDescriptorSetInfos(descriptorSetInfos),
		//mDefaultUniformBuffer(nullptr),
		GraphicsPipeline(info)
	{
		// Nothing
	}

	void VulkanGraphicsPipeline::SetupUniformStates(UInt32 count)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

		/*
		if (mDefaultUniformBuffer == nullptr)
		{
			UInt32 minSize = mpDevice->GetPhysicalDevice().GetPhysicalDeviceProperties().limits.minMemoryMapAlignment;
			
			if (minSize == 0)
			{
				minSize = 64;
			}

			mDefaultUniformBuffer = static_cast<VulkanBuffer*>
			(
				pGraphics->CreateBuffer(minSize, 
					BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
					BUFFER_ACCESS_DEVICE_LOCAL_BIT)
			);
		}
		*/

		for (UInt32 set = 0; set < mDescriptorSetInfos.Size(); set++)
		{
			VulkanDescriptorSetInfo& descriptorSetInfo = mDescriptorSetInfos[set];
			
			for (UInt32 i = 0; i < descriptorSetInfo.bindings.Size(); i++)
			{
				VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding = descriptorSetInfo.bindings[i];

				UInt32 binding = descriptorSetLayoutBinding.binding;

				Array<UniformState>& uniformStates = 
					mUniformMap.Put({ (UInt16)set, (UInt16)binding }, Array<UniformState>(count));

				for (UniformState& state : uniformStates)
				{
					state.SetupState(mpDevice, descriptorSetInfo, nullptr/*mDefaultUniformBuffer*/);
				}
			}
		}

		/*
		 
		if (mUniformStates.Size() != 0)
		{
			// wait for empty pipeline, destroy everything
		}
		 
		UInt32 setCount = mDescriptorSetInfos.Size();

		mUniformStates.Resize(count);
		mUniformBuffers.Resize(setCount);

		// @TODO: perhaps clean this up:
		Array<VkDescriptorSetLayout> tempLayouts(setCount);

		for (UInt32 i = 0; i < tempLayouts.Size(); i++)
		{
			tempLayouts[i] = mDescriptorSetInfos[i].vkDescriptorSetLayout;
		}

		for (UInt32 i = 0; i < count; i++)
		{
			VulkanUniformState& uniformState = mUniformStates[i];

			uniformState.descriptorSets.Resize(tempLayouts.Size());

			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType					= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool		= mpDevice->GetDescriptorPool();
			allocInfo.descriptorSetCount	= tempLayouts.Size();
			allocInfo.pSetLayouts			= tempLayouts.Data();

			if (vkAllocateDescriptorSets(mpDevice->GetDeviceHandle(), &allocInfo, uniformState.descriptorSets.Data()) != VK_SUCCESS)
			{
				// @Todo: error message
			}
		}

		for (UInt32 set = 0; set < setCount; set++)
		{
			UInt32 bufferSize = mDescriptorSetInfos[set].setSizeBytes * count; // ring buffer

			if (bufferSize != 0)
			{
				Buffer* pBuffer = pGraphics->CreateBuffer(bufferSize, BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
					BUFFER_ACCESS_HOST_COHERENT_BIT | BUFFER_ACCESS_HOST_VISIBLE_BIT);
				mUniformBuffers[set] = static_cast<VulkanBuffer*>(pBuffer);
			}
			else
			{
				mUniformBuffers[set] = nullptr;
			}
		}

		for (UInt32 i = 0; i < count; i++)
		{
			for (UInt32 set = 0; set < setCount; set++)
			{
				VkDescriptorBufferInfo* pVkBufferInfo	= nullptr;
				VkDescriptorImageInfo*	pVkImageInfo	= nullptr;

				VkDescriptorBufferInfo vkBufferInfo;
				if (mUniformBuffers[set] != nullptr)
				{
					pVkBufferInfo			= &vkBufferInfo;
					pVkBufferInfo->buffer	= mUniformBuffers[set]->GetVkBuffer();
					pVkBufferInfo->offset	= mDescriptorSetInfos[set].setSizeBytes * i;
					pVkBufferInfo->range	= mDescriptorSetInfos[set].setSizeBytes;
				}
				else
				{
					// error
				}

				UInt32 bindingCount = mDescriptorSetInfos[set].bindings.Size();

				Array<VkWriteDescriptorSet> descriptorWriteSets(bindingCount);

				for (UInt32 binding = 0; binding < bindingCount; binding++)
				{
					VkWriteDescriptorSet& descriptorWrite = descriptorWriteSets[binding];

					descriptorWrite.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					descriptorWrite.dstSet				= mUniformStates[i].descriptorSets[set];
					descriptorWrite.dstBinding			= mDescriptorSetInfos[set].bindings[binding].binding;
					descriptorWrite.dstArrayElement		= 0;
					descriptorWrite.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
					descriptorWrite.descriptorCount		= 1;
					descriptorWrite.pBufferInfo			= pVkBufferInfo;
					descriptorWrite.pImageInfo			= pVkImageInfo;
					descriptorWrite.pTexelBufferView	= nullptr;
				}

				vkUpdateDescriptorSets(mpDevice->GetDeviceHandle(), bindingCount, descriptorWriteSets.Data(), 0, VK_NULL_HANDLE);
			}
		}
		*/
	}

	UniformState& VulkanGraphicsPipeline::GetUniformState(UInt16 set, UInt16 binding, UInt32 frameIndex)
	{
		// TODO: Clean this up
		UniformSetBinding setBinding = { set, binding };
		Array<UniformState>* pUniformStates = mUniformMap.Get(setBinding);
		return (*pUniformStates)[frameIndex];
	}

	void UniformState::SetupState(VulkanDevice* pDevice, VulkanDescriptorSetInfo& info, VulkanBuffer* pBuffer)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType					= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool		= pDevice->GetDescriptorPool();
		allocInfo.descriptorSetCount	= 1;
		allocInfo.pSetLayouts			= &info.vkDescriptorSetLayout;

		if (vkAllocateDescriptorSets(pDevice->GetDeviceHandle(), &allocInfo, &descriptorSet) != VK_SUCCESS)
		{
			// @Todo: error message
		}

		/*
		bufferInfo.buffer	= pBuffer->GetVkBuffer();
		bufferInfo.offset	= 0;
		bufferInfo.range	= pBuffer->GetSize();

		this->pBuffer = pBuffer;
		*/

		const UInt32 bindingCount = info.bindings.Size();

		writeSets.Resize(bindingCount);

		for (UInt32 binding = 0; binding < bindingCount; binding++)
		{
			VkWriteDescriptorSet& descriptorWrite = writeSets[binding];

			descriptorWrite.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet				= descriptorSet;
			descriptorWrite.dstBinding			= info.bindings[binding].binding;
			descriptorWrite.dstArrayElement		= 0;
			descriptorWrite.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			descriptorWrite.descriptorCount		= 1;
			//descriptorWrite.pBufferInfo			= &bufferInfo;
			descriptorWrite.pBufferInfo			= nullptr;
			descriptorWrite.pImageInfo			= nullptr;
			descriptorWrite.pTexelBufferView	= nullptr;
		}

		//vkUpdateDescriptorSets(pDevice->GetDeviceHandle(), bindingCount, writeSets.Data(), 0, VK_NULL_HANDLE);

	}

	void UniformState::UpdateBuffer(VulkanDevice* pDevice, VulkanBuffer* pBuffer, UInt32 offset, UInt32 range)
	{
		bufferInfo.buffer	= pBuffer->GetVkBuffer();
		bufferInfo.offset	= offset;
		bufferInfo.range	= range;

		this->pBuffer = pBuffer;

		for (UInt32 binding = 0; binding < writeSets.Size(); binding++)
		{
			VkWriteDescriptorSet& descriptorWrite = writeSets[binding];
			descriptorWrite.pBufferInfo = &bufferInfo;
		}

		vkUpdateDescriptorSets(pDevice->GetDeviceHandle(), writeSets.Size(), writeSets.Data(), 0, VK_NULL_HANDLE);
	}
}

