#pragma once

#include "graphics/Pipeline.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanUniform.h"
#include "VulkanImage.h"
#include "VulkanSampler.h"
#include "util/Map.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	struct VulkanDescriptorSetInfo
	{
		UInt32								set;
		VkDescriptorSetLayout				vkDescriptorSetLayout;
		Array<VkDescriptorSetLayoutBinding> bindings;
		UInt32								sizeBytes;
	};

	struct VulkanDescriptorWriter
	{
		Array<VkWriteDescriptorSet>		descWrites;
		Array<VkDescriptorBufferInfo>	bufferInfos;
		Array<VkDescriptorImageInfo>	imageInfos;
		Map<UInt32, UInt32>				bindingTable;

		Bool8 operator==(const VulkanDescriptorWriter& writer);

		void SetupWriter(const VulkanDescriptorSetInfo& info);
		void SetDynamicBuffer(UInt32 binding, VkBuffer vkBuffer, UInt32 offset, UInt32 range);
		void SetImageSampler(UInt32 binding, VkImageView vkImageView, VkSampler vkSampler);
	
		void UpdateDescriptorSet(VulkanDevice* pDevice, VkDescriptorSet vkDescriptorSet);
	};

	class VulkanDescriptorCache
	{
	private:
		Map<VulkanDescriptorWriter, VkDescriptorSet> mDescriptors;

	public:
		VkDescriptorSet GetOrCreateDescriptorSet(VulkanDevice* pDevice, 
			VulkanDescriptorWriter& writer, VulkanDescriptorSetInfo& setInfo);
	};

	class QUARTZ_API VulkanGraphicsPipeline : public GraphicsPipeline
	{
	private:
		typedef UInt32 SetID;

		VkPipeline							mvkPipeline;
		VulkanDevice*						mpDevice;
		VkGraphicsPipelineCreateInfo		mvkPipelineInfo;
		Array<VulkanDescriptorSetInfo>		mDescriptorSetInfos;
		Array<VulkanDescriptorCache>		mDescriptorCaches;
		Map<SetID, VulkanDescriptorWriter>	mDescriptorWriters;

		VulkanSampler* mpDefaultSampler;

	public:
		VulkanGraphicsPipeline(VkPipeline vkPipeline, VulkanDevice* pDevice, const Array<VulkanDescriptorSetInfo>& descriptorSetInfos,
			VkGraphicsPipelineCreateInfo vkPipelineInfo, const GraphicsPipelineInfo& info);

		void SetupUniformStates(UInt32 count);

		VulkanDescriptorWriter* GetDescriptorWriter(UInt32 set);
		VkDescriptorSet GetCashedDescriptorSet(UInt32 set, VulkanDescriptorWriter& writer, UInt32 frameIndex);

		FORCE_INLINE VkPipeline								GetVkPipeline() { return mvkPipeline; }
		FORCE_INLINE VulkanSampler*							GetDefaultSampler() { return mpDefaultSampler; }
		FORCE_INLINE VkGraphicsPipelineCreateInfo			GetVkPipelineInfo() { return mvkPipelineInfo; }
		FORCE_INLINE const Array<VulkanDescriptorSetInfo>&	GetDescriptorSetInfos() const { return mDescriptorSetInfos; }
	};
}