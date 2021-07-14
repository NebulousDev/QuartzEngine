#include "VulkanPipeline.h"

#include "Engine.h"
#include "log/Log.h"

namespace Quartz
{
	template<>
	FORCE_INLINE UInt32 Hash<VulkanDescriptorWriter>(const VulkanDescriptorWriter& value)
	{
		UInt32 hash = 0;

		for (UInt32 i = 0; i < value.descWrites.Size(); i++)
		{
			hash ^= (UInt32)value.descWrites[i].pBufferInfo;//->buffer; // TODO: Not good
			hash ^= (UInt32)value.descWrites[i].pImageInfo;//->imageView;
		}

		return hash;
	}

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

		mDescriptorCaches.Resize(count);

		for (UInt32 setIndex = 0; setIndex < mDescriptorSetInfos.Size(); setIndex++)
		{
			VulkanDescriptorSetInfo& descriptorSetInfo = mDescriptorSetInfos[setIndex];
			
			VulkanDescriptorWriter descriptorWriter;
			descriptorWriter.SetupWriter(descriptorSetInfo);

			mDescriptorWriters.Put(setIndex, descriptorWriter);
		}

		VkSampler vkSampler;

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType					= VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter				= VK_FILTER_LINEAR;
		samplerInfo.minFilter				= VK_FILTER_LINEAR;
		samplerInfo.addressModeU			= VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV			= VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW			= VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable		= VK_TRUE;
		samplerInfo.maxAnisotropy			= mpDevice->GetPhysicalDevice().GetPhysicalDeviceProperties().limits.maxSamplerAnisotropy;
		samplerInfo.borderColor				= VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates	= VK_FALSE;
		samplerInfo.compareEnable			= VK_FALSE;
		samplerInfo.compareOp				= VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode				= VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerInfo.mipLodBias				= 0.0f;
		samplerInfo.minLod					= 0.0f;
		samplerInfo.maxLod					= 8.0f;

		// TODO: Samplers should not be created here
		if (vkCreateSampler(mpDevice->GetDeviceHandle(), &samplerInfo, nullptr, &vkSampler) != VK_SUCCESS)
		{
			Log::Error("Failed to create texture sampler!");
		}

		mpDefaultSampler = new VulkanSampler(vkSampler);
	}

	VulkanDescriptorWriter* VulkanGraphicsPipeline::GetDescriptorWriter(UInt32 set)
	{
		return mDescriptorWriters.Get(set);
	}

	VkDescriptorSet VulkanGraphicsPipeline::GetCashedDescriptorSet(UInt32 set, VulkanDescriptorWriter& writer, UInt32 frameIndex)
	{
		VulkanDescriptorSetInfo& descriptorInfo = mDescriptorSetInfos[set]; // TODO: Set might not always align with the set index!
		VulkanDescriptorCache& descriptorCache = mDescriptorCaches[frameIndex];
		//VulkanDescriptorWriter* pDescriptorWriter = mDescriptorWriters.Get(set);

		return descriptorCache.GetOrCreateDescriptorSet(mpDevice, writer, descriptorInfo);
	}

	Bool8 VulkanDescriptorWriter::operator==(const VulkanDescriptorWriter& writer)
	{
		for (UInt32 i = 0; i < writer.descWrites.Size(); i++)
		{
			// TODO: what is this awful mess...
			if (writer.bufferInfos[i].buffer != bufferInfos[i].buffer ||
				writer.imageInfos[i].imageView != imageInfos[i].imageView)
			{
				return false;
			}
		}

		return true;
	}

	void VulkanDescriptorWriter::SetupWriter(const VulkanDescriptorSetInfo& info)
	{
		const UInt32 bindingCount = info.bindings.Size();

		descWrites.Resize(bindingCount);
		bufferInfos.Resize(bindingCount);
		imageInfos.Resize(bindingCount);

		for (UInt32 binding = 0; binding < bindingCount; binding++)
		{
			const VkDescriptorSetLayoutBinding& descriptorBinding = info.bindings[binding];
			VkWriteDescriptorSet& descriptorWrite = descWrites[binding];

			descriptorWrite.sType				= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet				= VK_NULL_HANDLE;
			descriptorWrite.dstBinding			= descriptorBinding.binding;
			descriptorWrite.dstArrayElement		= 0;
			descriptorWrite.descriptorType		= descriptorBinding.descriptorType;
			descriptorWrite.descriptorCount		= descriptorBinding.descriptorCount;
			descriptorWrite.pBufferInfo			= &bufferInfos[binding];
			descriptorWrite.pImageInfo			= &imageInfos[binding];
			descriptorWrite.pTexelBufferView	= nullptr;

			bindingTable.Put(descriptorBinding.binding, binding);
		}
	}

	void VulkanDescriptorWriter::SetDynamicBuffer(UInt32 binding, VkBuffer vkBuffer, UInt32 offset, UInt32 range)
	{
		//TODO: Assert correct type

		UInt32* pBindIndex = bindingTable.Get(binding);

		if (pBindIndex)
		{
			VkDescriptorBufferInfo& bufferInfo = bufferInfos[*pBindIndex];
			bufferInfo.buffer	= vkBuffer;
			bufferInfo.offset	= offset;
			bufferInfo.range	= range;

			descWrites[*pBindIndex].pBufferInfo = &bufferInfo;
		}
		else
		{
			// TODO: No binding, Error
		}
	}

	void VulkanDescriptorWriter::SetImageSampler(UInt32 binding, VkImageView vkImageView, VkSampler vkSampler)
	{
		UInt32* pBindIndex = bindingTable.Get(binding);

		if (pBindIndex)
		{
			VkDescriptorImageInfo& imageInfo = imageInfos[*pBindIndex];
			imageInfo.imageView		= vkImageView;
			imageInfo.imageLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL; // TODO: Not always the case?
			imageInfo.sampler		= vkSampler;

			descWrites[*pBindIndex].pImageInfo = &imageInfo;
		}
		else
		{
			// TODO: Error
		}
	}

	void VulkanDescriptorWriter::UpdateDescriptorSet(VulkanDevice* pDevice, VkDescriptorSet vkDescriptorSet)
	{
		for (VkWriteDescriptorSet& write : descWrites)
		{
			write.dstSet = vkDescriptorSet;
		}

		vkUpdateDescriptorSets(pDevice->GetDeviceHandle(), descWrites.Size(), descWrites.Data(), 0, VK_NULL_HANDLE);
	}

	VkDescriptorSet VulkanDescriptorCache::GetOrCreateDescriptorSet(VulkanDevice* pDevice, 
		VulkanDescriptorWriter& writer, VulkanDescriptorSetInfo& setInfo)
	{
		VkDescriptorSet* pvkDescriptorSet = mDescriptors.Get(writer);

		if (pvkDescriptorSet)
		{
			return *pvkDescriptorSet;
		}
		else
		{
			VkDescriptorSet vkDescriptorSet;

			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType					= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool		= pDevice->GetDescriptorPool();
			allocInfo.descriptorSetCount	= 1;
			allocInfo.pSetLayouts			= &setInfo.vkDescriptorSetLayout;

			if (vkAllocateDescriptorSets(pDevice->GetDeviceHandle(), &allocInfo, &vkDescriptorSet) != VK_SUCCESS)
			{
				// @Todo: error message
			}

			//writer.UpdateDescriptorSet(pDevice, vkDescriptorSet);

			mDescriptors.Put(writer, vkDescriptorSet);

			Log::Debug("VkDescriptorSet [%p] created.", vkDescriptorSet);

			return vkDescriptorSet;
		}
	}
}

