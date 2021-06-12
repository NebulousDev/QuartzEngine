#include "VulkanPipeline.h"

#include "Engine.h"

namespace Quartz
{
	VulkanGraphicsPipeline::VulkanGraphicsPipeline(VkPipeline vkPipeline, VulkanDevice* pDevice, const Array<VulkanDescriptorSetInfo>& descriptorSetInfos,
		VkGraphicsPipelineCreateInfo vkPipelineInfo, const GraphicsPipelineInfo& info)
		:mvkPipeline(vkPipeline),
		mpDevice(pDevice),
		mvkPipelineInfo(vkPipelineInfo),
		mDescriptorSetInfos(descriptorSetInfos),
		GraphicsPipeline(info)
	{
		// Nothing
	}

	void VulkanGraphicsPipeline::SetupUniformStates(UInt32 count)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

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
					descriptorWrite.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					descriptorWrite.descriptorCount		= 1;
					descriptorWrite.pBufferInfo			= pVkBufferInfo;
					descriptorWrite.pImageInfo			= pVkImageInfo;
					descriptorWrite.pTexelBufferView	= nullptr;
				}

				vkUpdateDescriptorSets(mpDevice->GetDeviceHandle(), bindingCount, descriptorWriteSets.Data(), 0, VK_NULL_HANDLE);
			}
		}
	}
}

