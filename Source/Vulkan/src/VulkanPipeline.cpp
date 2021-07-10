#include "VulkanPipeline.h"

#include "Engine.h"
#include "log/Log.h"

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

		for (UInt32 setIndex = 0; setIndex < mDescriptorSetInfos.Size(); setIndex++)
		{
			VulkanDescriptorSetInfo& descriptorSetInfo = mDescriptorSetInfos[setIndex];
			
			for (UInt32 i = 0; i < descriptorSetInfo.bindings.Size(); i++)
			{
				VkDescriptorSetLayoutBinding& descriptorSetLayoutBinding = descriptorSetInfo.bindings[i];

				UInt32 binding = descriptorSetLayoutBinding.binding;

				Array<UniformState>& uniformStates = 
					mUniformMap.Put(descriptorSetInfo.set, Array<UniformState>(count));

				for (UniformState& state : uniformStates)
				{
					state.SetupState(mpDevice, descriptorSetInfo);
				}
			}
		}
	}

	UniformState& VulkanGraphicsPipeline::GetUniformState(UInt16 set, UInt32 frameIndex)
	{
		Array<UniformState>* pUniformStates = mUniformMap.Get(set);
		return (*pUniformStates)[frameIndex];
	}

	void UniformState::SetupState(VulkanDevice* pDevice, VulkanDescriptorSetInfo& info)
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

		const UInt32 bindingCount = info.bindings.Size();

		writeSets.Resize(bindingCount);
		bufferInfos.Resize(bindingCount);
		imageInfos.Resize(bindingCount);

		for (UInt32 binding = 0; binding < bindingCount; binding++)
		{
			VkDescriptorSetLayoutBinding&	descriptorBinding	= info.bindings[binding];
			VkWriteDescriptorSet&			descriptorWrite		= writeSets[binding];

			descriptorWrite.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet				= descriptorSet;
			descriptorWrite.dstBinding			= descriptorBinding.binding;
			descriptorWrite.dstArrayElement		= 0;
			descriptorWrite.descriptorType		= descriptorBinding.descriptorType;
			descriptorWrite.descriptorCount		= descriptorBinding.descriptorCount;
			descriptorWrite.pBufferInfo			= nullptr;
			descriptorWrite.pImageInfo			= nullptr;
			descriptorWrite.pTexelBufferView	= nullptr;

			bindIndexMap[descriptorBinding.binding] = binding;
		}

		this->set = info.set;
	}

	void UniformState::SetBuffer(VulkanDevice* pDevice, VulkanBuffer* pBuffer, UInt32 binding, UInt32 offset, UInt32 range)
	{
		UInt32* pBindIndex = bindIndexMap.Get(binding);

		if (pBindIndex)
		{
			VkDescriptorBufferInfo& bufferInfo = bufferInfos[*pBindIndex];
			bufferInfo.buffer	= pBuffer->GetVkBuffer();
			bufferInfo.offset	= offset;
			bufferInfo.range	= range;

			VkWriteDescriptorSet& descriptorWrite = writeSets[*pBindIndex];
			descriptorWrite.pBufferInfo = &bufferInfo;
		}
		else
		{
			// TODO: Error
		}
	}

	void UniformState::SetImageSampler(VulkanDevice* pDevice, VulkanImageView* pImageView, UInt32 binding)
	{
		UInt32* pBindIndex = bindIndexMap.Get(binding);

		if (pBindIndex)
		{
			VkSampler vkSampler;

			VkSamplerCreateInfo samplerInfo{};
			samplerInfo.sType					= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			samplerInfo.magFilter				= VK_FILTER_NEAREST;
			samplerInfo.minFilter				= VK_FILTER_NEAREST;
			samplerInfo.addressModeU			= VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeV			= VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.addressModeW			= VK_SAMPLER_ADDRESS_MODE_REPEAT;
			samplerInfo.anisotropyEnable		= VK_TRUE;
			samplerInfo.maxAnisotropy			= pDevice->GetPhysicalDevice().GetPhysicalDeviceProperties().limits.maxSamplerAnisotropy;
			samplerInfo.borderColor				= VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			samplerInfo.unnormalizedCoordinates	= VK_FALSE;
			samplerInfo.compareEnable			= VK_FALSE;
			samplerInfo.compareOp				= VK_COMPARE_OP_ALWAYS;
			samplerInfo.mipmapMode				= VK_SAMPLER_MIPMAP_MODE_LINEAR;
			samplerInfo.mipLodBias				= 0.0f;
			samplerInfo.minLod					= 0.0f;
			samplerInfo.maxLod					= 0.0f;

			// TODO: Samplers should not be created here
			if (vkCreateSampler(pDevice->GetDeviceHandle(), &samplerInfo, nullptr, &vkSampler) != VK_SUCCESS)
			{
				Log::Error("Failed to create texture sampler!");
			}

			VkDescriptorImageInfo& imageInfo = imageInfos[*pBindIndex];
			imageInfo.imageView		= pImageView->GetVkImageView();
			imageInfo.imageLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // TODO: NOT ALWAYS THE CASE!!
			imageInfo.sampler		= vkSampler;

			VkWriteDescriptorSet& descriptorWrite = writeSets[*pBindIndex];
			descriptorWrite.pImageInfo = &imageInfo;
		}
		else
		{
			// TODO: Error
		}
	}

	void UniformState::UpdateDescriptorSets(VulkanDevice* pDevice)
	{
		vkUpdateDescriptorSets(pDevice->GetDeviceHandle(), writeSets.Size(), writeSets.Data(), 0, VK_NULL_HANDLE);
	}
}

