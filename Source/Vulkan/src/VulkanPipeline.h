#pragma once

#include "graphics2/Pipeline.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"

#include <vulkan/vulkan.h>

#define MakeUniformSetBinding(set, binding) (set & binding << 16)

namespace Quartz
{
	struct UniformSetBinding
	{
		UInt16 set		: 16;
		UInt16 binding	: 16;
	};

	struct VulkanUniformState
	{
		Array<VkDescriptorSet>		descriptorSets;
		Array<VkWriteDescriptorSet>	writeSets;
	};

	struct VulkanDescriptorSetInfo
	{
		VkDescriptorSetLayout				vkDescriptorSetLayout;
		Array<VkDescriptorSetLayoutBinding> bindings;
		UInt32								setSizeBytes;
	};

	class QUARTZ_API VulkanGraphicsPipeline : public GraphicsPipeline
	{
	private:
		VkPipeline						mvkPipeline;
		VulkanDevice*					mpDevice;
		VkGraphicsPipelineCreateInfo	mvkPipelineInfo;
		Array<VulkanDescriptorSetInfo>	mDescriptorSetInfos;
		Array<VulkanUniformState>		mUniformStates;
		Array<VulkanBuffer*>			mUniformBuffers; // ring buffers

	public:
		VulkanGraphicsPipeline(VkPipeline vkPipeline, VulkanDevice* pDevice, const Array<VulkanDescriptorSetInfo>& descriptorSetInfos,
			VkGraphicsPipelineCreateInfo vkPipelineInfo, const GraphicsPipelineInfo& info);

		void SetupUniformStates(UInt32 count);

		FORCE_INLINE VkPipeline							GetVkPipeline() { return mvkPipeline; }
		FORCE_INLINE VkGraphicsPipelineCreateInfo		GetVkPipelineInfo() { return mvkPipelineInfo; }
		FORCE_INLINE const Array<VulkanUniformState>&	GetUniformStates() { return mUniformStates; }
	};
}