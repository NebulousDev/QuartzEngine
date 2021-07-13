#pragma once

#include "graphics/Pipeline.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanUniform.h"
#include "VulkanImage.h"
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

	struct UniformState
	{
		UInt32							set;
		VkDescriptorSet					descriptorSet;
		Map<UInt32, UInt32>				bindIndexMap;
		Array<VkDescriptorBufferInfo>	bufferInfos;
		Array<VkDescriptorImageInfo>	imageInfos;
		Array<VkWriteDescriptorSet>		writeSets;

		void SetupState(VulkanDevice* pDevice, VulkanDescriptorSetInfo& info);

		void SetBuffer(VulkanDevice* pDevice, VulkanBuffer* pBuffer, UInt32 binding, UInt32 offset, UInt32 range);
		void SetImageSampler(VulkanDevice* pDevice, VulkanImageView* pImageView, UInt32 binding);

		void UpdateDescriptorSets(VulkanDevice* pDevice);
	};

	class QUARTZ_API VulkanGraphicsPipeline : public GraphicsPipeline
	{
	private:
		VkPipeline							mvkPipeline;
		VulkanDevice*						mpDevice;
		VkGraphicsPipelineCreateInfo		mvkPipelineInfo;
		Array<VulkanDescriptorSetInfo>		mDescriptorSetInfos;
		Map<UInt32, Array<UniformState>>	mUniformMap;

	public:
		VulkanGraphicsPipeline(VkPipeline vkPipeline, VulkanDevice* pDevice, const Array<VulkanDescriptorSetInfo>& descriptorSetInfos,
			VkGraphicsPipelineCreateInfo vkPipelineInfo, const GraphicsPipelineInfo& info);

		void SetupUniformStates(UInt32 count);

		UniformState& GetUniformState(UInt16 set, UInt32 frameIndex);

		FORCE_INLINE VkPipeline								GetVkPipeline() { return mvkPipeline; }
		FORCE_INLINE VkGraphicsPipelineCreateInfo			GetVkPipelineInfo() { return mvkPipelineInfo; }
		FORCE_INLINE const Array<VulkanDescriptorSetInfo>&	GetDescriptorSetInfos() const { return mDescriptorSetInfos; }
	};
}