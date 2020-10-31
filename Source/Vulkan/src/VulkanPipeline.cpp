#include "VulkanPipeline.h"

#include "log/Log.h"


namespace Quartz
{
	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(VulkanDevice& device, const Array<VulkanDescriptorSetLayoutBinding>& bindings)
		: VulkanDescriptorSetLayout()
	{
		VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo = {};
		descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptorSetLayoutInfo.flags = 0;
		descriptorSetLayoutInfo.bindingCount = bindings.Size();

		//NOTE: This will not work if VulkanDescriptorSetLayoutBinding struct is changed!
		descriptorSetLayoutInfo.pBindings = (const VkDescriptorSetLayoutBinding*)bindings.Data();

		if (vkCreateDescriptorSetLayout(device.GetDeviceHandle(), &descriptorSetLayoutInfo, nullptr, &mDescriptorSetLayout) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan descriptor set layout: vkCreateDescriptorSetLayout failed!");
			return;
		}

		mpParentDevice = &device;
		mIsValidDescriptorSetLayout = true;
	}

	void VulkanDescriptorSetLayout::Destroy()
	{
		if (mIsValidDescriptorSetLayout)
		{
			vkDestroyDescriptorSetLayout(mpParentDevice->GetDeviceHandle(), mDescriptorSetLayout, nullptr);
		}
	}

	VulkanPipelineLayout::VulkanPipelineLayout(VulkanDevice& device, const Array<VulkanDescriptorSetLayout>& descriptorSets)
		: VulkanPipelineLayout()
	{
		VkPipelineLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layoutInfo.flags = 0;

		Array<VkDescriptorSetLayout> mDescriptorSetLayouts;
		for (VulkanDescriptorSetLayout layout : descriptorSets)
		{
			mDescriptorSetLayouts.PushBack(layout.GetDescriptorSetLayoutHandle());
		}

		layoutInfo.setLayoutCount = mDescriptorSetLayouts.Size();
		layoutInfo.pSetLayouts = mDescriptorSetLayouts.Data();

		// TODO: Support push constants?
		layoutInfo.pushConstantRangeCount = 0;
		layoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(device.GetDeviceHandle(), &layoutInfo, nullptr, &mPipelineLayout) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan pipeline layout: vkCreatePipelineLayout failed!");
			return;
		}

		mpParentDevice = &device;
		mIsValidPipelineLayout = true;
	}

	void VulkanPipelineLayout::Destroy()
	{
		if (mIsValidPipelineLayout)
		{
			vkDestroyPipelineLayout(mpParentDevice->GetDeviceHandle(), mPipelineLayout, nullptr);
		}
	}

	VulkanGraphicsPipeline::VulkanGraphicsPipeline(VulkanDevice& device, const VulkanGraphicsPipelineState& pipelineState, const VulkanGraphicsPipeline* pParentPipeline)
		: VulkanGraphicsPipeline()
	{
		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		if (pParentPipeline != nullptr && pParentPipeline->mPipelineState.flags & VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT)
		{
			pipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
			pipelineInfo.basePipelineHandle = pParentPipeline->GetPipelineHandle();
		}

		/* Shader Stage State */

		Array<VkPipelineShaderStageCreateInfo> shaderStageInfos;

		VkPipelineShaderStageCreateInfo shaderStageInfo = {};
		shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		shaderStageInfo.flags = 0;
		shaderStageInfo.pSpecializationInfo = nullptr;
		shaderStageInfo.pNext = nullptr;

		if(pipelineState.pVertexShader != nullptr)
		{
			shaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			shaderStageInfo.module = pipelineState.pVertexShader->GetShaderModuleHandle();
			shaderStageInfo.pName = pipelineState.pVertexShader->GetEntryName();

			shaderStageInfos.PushBack(shaderStageInfo);
		}
		else
		{
			Log.Error("Failed to create vulkan graphics pipeline: No required vertex shader attached!");
			return;
		}

		if (pipelineState.pPixelShader != nullptr)
		{
			shaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			shaderStageInfo.module = pipelineState.pPixelShader->GetShaderModuleHandle();
			shaderStageInfo.pName = pipelineState.pPixelShader->GetEntryName();

			shaderStageInfos.PushBack(shaderStageInfo);
		}
		else
		{
			Log.Error("Failed to create vulkan graphics pipeline: No required pixel shader attached!");
			return;
		}

		pipelineInfo.stageCount = shaderStageInfos.Size();
		pipelineInfo.pStages = shaderStageInfos.Data();

		/* Vertex Input State */

		VkPipelineVertexInputStateCreateInfo vertexInputStateInfo = {};
		vertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputStateInfo.flags = 0;

		Array<VkVertexInputBindingDescription> bindingDescriptions;
		for (const VulkanVertexBinding& binding : pipelineState.vertexBindings)
		{
			VkVertexInputBindingDescription bindingDescription;
			bindingDescription.binding = binding.binding;
			bindingDescription.stride = binding.stride;
			bindingDescription.inputRate = binding.inputRate;

			bindingDescriptions.PushBack(bindingDescription);
		}

		vertexInputStateInfo.vertexBindingDescriptionCount = bindingDescriptions.Size();
		vertexInputStateInfo.pVertexBindingDescriptions = bindingDescriptions.Data();

		Array<VkVertexInputAttributeDescription> attributeDescriptions;
		for (const VulkanVertexAttribute& attrib : pipelineState.vertexAttributes)
		{
			VkVertexInputAttributeDescription attributeDescription;
			attributeDescription.binding = attrib.binding;
			attributeDescription.location = attrib.location;
			attributeDescription.format = attrib.format;
			attributeDescription.offset = attrib.offset;

			attributeDescriptions.PushBack(attributeDescription);
		}

		vertexInputStateInfo.vertexAttributeDescriptionCount = attributeDescriptions.Size();
		vertexInputStateInfo.pVertexAttributeDescriptions = attributeDescriptions.Data();

		pipelineInfo.pVertexInputState = &vertexInputStateInfo;

		/* Input Assembly State */

		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo = {};
		inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssemblyInfo.flags = 0;
		inputAssemblyInfo.topology = pipelineState.topology;

		pipelineInfo.pInputAssemblyState = &inputAssemblyInfo;

		/* Tesselation State */

		pipelineInfo.pTessellationState = nullptr;

		/* Viewport State */

		VkPipelineViewportStateCreateInfo viewportInfo = {};
		viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportInfo.flags = 0;

		Array<VkViewport> viewports;
		for (const VkViewport& viewport : pipelineState.viewports)
		{
			viewports.PushBack(viewport);
		}

		viewportInfo.viewportCount = viewports.Size();
		viewportInfo.pViewports = viewports.Data();

		Array<VkRect2D> scissors;
		for (const VkRect2D& scissor : pipelineState.scissors)
		{
			scissors.PushBack(scissor);
		}

		viewportInfo.scissorCount = scissors.Size();
		viewportInfo.pScissors = scissors.Data();

		pipelineInfo.pViewportState = &viewportInfo;

		/* Rasterization State */

		VkPipelineRasterizationStateCreateInfo rasterizationInfo = {};
		rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationInfo.flags = 0;
		rasterizationInfo.depthClampEnable = VK_FALSE;
		rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		rasterizationInfo.polygonMode = pipelineState.polygonMode;
		rasterizationInfo.cullMode = pipelineState.cullMode;
		rasterizationInfo.frontFace = pipelineState.frontFace;
		rasterizationInfo.depthBiasEnable = 0;
		rasterizationInfo.depthBiasConstantFactor = 0;
		rasterizationInfo.depthBiasClamp = 0;
		rasterizationInfo.depthBiasSlopeFactor = 0;
		rasterizationInfo.lineWidth = pipelineState.lineWidth;

		pipelineInfo.pRasterizationState = &rasterizationInfo;

		/* Multisample State */

		UInt32 multisamples = pipelineState.multisamples;
		UInt32 maxMultisamples = device.GetPhysicalDevice().GetPhysicalDeviceProperties().limits.framebufferColorSampleCounts;

		if (multisamples == 0)
		{
			multisamples = 1;
			Log.Warning("Invalid zero multisample value in pipeline creation, setting multisamples to 1");
		}

		if (pipelineState.multisamples > maxMultisamples)
		{
			multisamples = maxMultisamples;
			Log.Warning("Invalid maximum multisample value in pipeline creation, setting multisamples to maxMultisamples");
		}

		VkPipelineMultisampleStateCreateInfo multisampleInfo = {};
		multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleInfo.flags = 0;
		multisampleInfo.rasterizationSamples = (VkSampleCountFlagBits)multisamples;
		multisampleInfo.sampleShadingEnable = VK_FALSE;
		multisampleInfo.minSampleShading = 0.0f;
		multisampleInfo.pSampleMask = nullptr;
		multisampleInfo.alphaToCoverageEnable = VK_FALSE;
		multisampleInfo.alphaToOneEnable = VK_FALSE;

		pipelineInfo.pMultisampleState = &multisampleInfo;

		/* Depth Stencil State */

		VkPipelineDepthStencilStateCreateInfo depthStencilInfo = {};
		depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		depthStencilInfo.flags = 0;
		depthStencilInfo.depthTestEnable = pipelineState.enableDepthTest;
		depthStencilInfo.depthWriteEnable = pipelineState.enableDepthWrite;
		depthStencilInfo.depthCompareOp = pipelineState.depthCompareOp;
		depthStencilInfo.depthBoundsTestEnable = pipelineState.enableDepthBoundsTest;
		depthStencilInfo.stencilTestEnable = pipelineState.enableStencilTest;
		depthStencilInfo.front.failOp = pipelineState.frontFailOp;
		depthStencilInfo.front.passOp = pipelineState.frontPassOp;
		depthStencilInfo.front.depthFailOp = pipelineState.frontDepthFailOp;
		depthStencilInfo.front.compareOp = pipelineState.frontCompareOp;
		depthStencilInfo.front.compareMask = pipelineState.frontCompareMask;
		depthStencilInfo.front.writeMask = pipelineState.frontWriteMask;
		depthStencilInfo.front.reference = pipelineState.frontReference;
		depthStencilInfo.back.failOp = pipelineState.backFailOp;
		depthStencilInfo.back.passOp = pipelineState.backPassOp;
		depthStencilInfo.back.depthFailOp = pipelineState.backDepthFailOp;
		depthStencilInfo.back.compareOp = pipelineState.backCompareOp;
		depthStencilInfo.back.compareMask = pipelineState.backCompareMask;
		depthStencilInfo.back.writeMask = pipelineState.backWriteMask;
		depthStencilInfo.back.reference = pipelineState.backReference;
		depthStencilInfo.minDepthBounds = pipelineState.minDepth;
		depthStencilInfo.maxDepthBounds = pipelineState.maxDepth;

		pipelineInfo.pDepthStencilState = &depthStencilInfo;

		/* Color Blend State */

		VkPipelineColorBlendStateCreateInfo colorBlendInfo = {};
		colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlendInfo.flags = 0;
		colorBlendInfo.logicOpEnable = pipelineState.enableColorBlendLogicOp;
		colorBlendInfo.logicOp = pipelineState.colorBlendLogicOp;

		Array<VkPipelineColorBlendAttachmentState> blendAttachments;
		for (const VulkanBlendAttachment& blendAttachment : pipelineState.colorBlendAttachments)
		{
			VkPipelineColorBlendAttachmentState blendAttachmentState = {};
			blendAttachmentState.blendEnable = blendAttachment.enableBlend;
			blendAttachmentState.srcColorBlendFactor = blendAttachment.srcColorBlendFactor;
			blendAttachmentState.dstColorBlendFactor = blendAttachment.dstColorBlendFactor;
			blendAttachmentState.colorBlendOp = blendAttachment.colorBlendOp;
			blendAttachmentState.srcAlphaBlendFactor = blendAttachment.srcAlphaBlendFactor;
			blendAttachmentState.dstAlphaBlendFactor = blendAttachment.dstAlphaBlendFactor;
			blendAttachmentState.alphaBlendOp = blendAttachment.alphaBlendOp;
			blendAttachmentState.colorWriteMask = blendAttachment.colorWriteMask;

			blendAttachments.PushBack(blendAttachmentState);
		}

		colorBlendInfo.attachmentCount = blendAttachments.Size();
		colorBlendInfo.pAttachments = blendAttachments.Data();

		// TODO: expose blend constants to the pipeline state?
		colorBlendInfo.blendConstants[0] = 1.0f;
		colorBlendInfo.blendConstants[1] = 1.0f;
		colorBlendInfo.blendConstants[2] = 1.0f;
		colorBlendInfo.blendConstants[3] = 1.0f;

		pipelineInfo.pColorBlendState = &colorBlendInfo;

		/* Dynamic States */

		// TODO: Support dynamic states
		pipelineInfo.pDynamicState = nullptr;

		/* Pipeline Layout */

		pipelineInfo.layout = pipelineState.pipelineLayout.GetPipelineLayoutHandle();

		/* Render Pass */

		pipelineInfo.renderPass = pipelineState.renderPass.GetRenderPassHandle();
		pipelineInfo.subpass = pipelineState.subpassIndex;

		if (vkCreateGraphicsPipelines(device.GetDeviceHandle(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &mPipeline) != VK_SUCCESS)
		{
			Log.Error("Failed to create vulkan graphics pipeline: vkCreateGraphicsPipelines failed!");
			return;
		}

		mPipelineState = pipelineState;
		mpParentDevice = &device;
		mIsValidPipeline = true;
	}

	void VulkanGraphicsPipeline::Destroy()
	{
		if (mIsValidPipeline)
		{
			vkDestroyPipeline(mpParentDevice->GetDeviceHandle(), mPipeline, nullptr);
		}
	}
}

