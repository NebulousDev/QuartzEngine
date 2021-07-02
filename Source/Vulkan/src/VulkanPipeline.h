#pragma once

#include "graphics2/Pipeline.h"
#include "VulkanDevice.h"
#include "VulkanBuffer.h"
#include "VulkanUniform.h"
#include "util/Map.h"

#include <vulkan/vulkan.h>

#define MakeUniformSetBinding(set, binding) (set & binding << 16)

namespace Quartz
{
	struct UniformSetBinding
	{
		union
		{
			struct
			{
				UInt16 set		: 16;
				UInt16 binding	: 16;
			};

			UInt32 intValue;
		};

		Bool8 operator==(const UniformSetBinding& other);
	};

	struct VulkanDescriptorSetInfo
	{
		VkDescriptorSetLayout				vkDescriptorSetLayout;
		Array<VkDescriptorSetLayoutBinding> bindings;
		UInt32								setSizeBytes;
	};

	struct UniformState
	{
		VulkanBuffer*				pBuffer;
		VkDescriptorBufferInfo		bufferInfo;
		VkDescriptorSet				descriptorSet;
		Array<VkWriteDescriptorSet>	writeSets;

		void SetupState(VulkanDevice* pDevice, VulkanDescriptorSetInfo& info, VulkanBuffer* pBuffer);
		void UpdateBuffer(VulkanDevice* pDevice, VulkanBuffer* pBuffer, UInt32 offset, UInt32 range);
	};

	class QUARTZ_API VulkanGraphicsPipeline : public GraphicsPipeline
	{
	private:
		VkPipeline									mvkPipeline;
		VulkanDevice*								mpDevice;
		VkGraphicsPipelineCreateInfo				mvkPipelineInfo;
		Array<VulkanDescriptorSetInfo>				mDescriptorSetInfos;
		Map<UniformSetBinding, Array<UniformState>> mUniformMap;

	public:
		VulkanGraphicsPipeline(VkPipeline vkPipeline, VulkanDevice* pDevice, const Array<VulkanDescriptorSetInfo>& descriptorSetInfos,
			VkGraphicsPipelineCreateInfo vkPipelineInfo, const GraphicsPipelineInfo& info);

		void SetupUniformStates(UInt32 count);

		UniformState& GetUniformState(UInt16 set, UInt16 binding, UInt32 frameIndex);

		FORCE_INLINE VkPipeline							GetVkPipeline() { return mvkPipeline; }
		FORCE_INLINE VkGraphicsPipelineCreateInfo		GetVkPipelineInfo() { return mvkPipelineInfo; }
	};
}