#pragma once

#include "Common.h"
#include "util\Array.h"

#include "graphics\GFXPipelines.h"
#include "VulkanShader.h"
#include "VulkanDevice.h"
#include "VulkanRenderPass.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	struct QUARTZ_API VulkanVertexBinding
	{
		UInt32				binding;
		UInt32				stride;
		VkVertexInputRate	inputRate;
	};

	struct QUARTZ_API VulkanVertexAttribute
	{
		UInt32		binding;
		UInt32		location;
		VkFormat	format;
		UInt32		offset;
	};

	struct QUARTZ_API VulkanViewport
	{
		Float32 x;
		Float32 y;
		Float32 width;
		Float32 height;
		Float32 minDepth;
		Float32 maxDepth;
	};

	struct QUARTZ_API VulkanScissor
	{
		UInt32 x;
		UInt32 y;
		UInt32 width;
		UInt32 height;
	};

	struct QUARTZ_API VulkanBlendAttachment
	{
		Bool8 enableBlend;
		VkBlendFactor srcColorBlendFactor;
		VkBlendFactor dstColorBlendFactor;
		VkBlendOp colorBlendOp;
		VkBlendFactor srcAlphaBlendFactor;
		VkBlendFactor dstAlphaBlendFactor;
		VkBlendOp alphaBlendOp;
		VkColorComponentFlags colorWriteMask;
	};

	struct QUARTZ_API VulkanDescriptorSetLayoutBinding
	{
		UInt32 binding;
		VkDescriptorType descriptorType;
		UInt32 descriptorCount; //NOTE: uniform array size
		VkShaderStageFlags stageFlags;

		// Potentially Array<...>?
		const VkSampler* pImmutableSamplers;
	};

	class QUARTZ_API VulkanDescriptorSetLayout
	{
	private:
		VulkanDevice*			mpParentDevice;
		VkDescriptorSetLayout	mDescriptorSetLayout;
		Bool8					mIsValidDescriptorSetLayout;

	public:
		VulkanDescriptorSetLayout() = default;
		VulkanDescriptorSetLayout(VulkanDevice& device, const Array<VulkanDescriptorSetLayoutBinding>& bindings);

		VulkanDevice& GetParentDevice() { return *mpParentDevice; }
		VkDescriptorSetLayout GetDescriptorSetLayoutHandle() const { return mDescriptorSetLayout; }

		Bool8 IsValidDescriptorSetLayout() const { return mIsValidDescriptorSetLayout; }

		void Destroy();
	};

	class QUARTZ_API VulkanPipelineLayout
	{
	private:
		VulkanDevice*		mpParentDevice;
		VkPipelineLayout	mPipelineLayout;
		Bool8				mIsValidPipelineLayout;

	public:
		VulkanPipelineLayout() = default;
		VulkanPipelineLayout(VulkanDevice& device, const Array<VulkanDescriptorSetLayout>& descriptorSets);

		VulkanDevice& GetParentDevice() { return *mpParentDevice; }
		VkPipelineLayout GetPipelineLayoutHandle() const { return mPipelineLayout; }

		Bool8 IsValidPipelineLayout() const { return mIsValidPipelineLayout; }

		void Destroy();
	};

	struct QUARTZ_API VulkanGraphicsPipelineState
	{
		VkPipelineCreateFlags flags;

		VulkanShader* pVertexShader;
		VulkanShader* pHullShader;
		VulkanShader* pDomainShader;
		VulkanShader* pGeometryShader;
		VulkanShader* pPixelShader;

		Array<VulkanVertexBinding> vertexBindings;
		Array<VulkanVertexAttribute> vertexAttributes;

		VkPrimitiveTopology topology;

		Array<VulkanViewport> viewports;
		Array<VulkanScissor> scissors;

		VkPolygonMode polygonMode;
		VkCullModeFlags cullMode;
		VkFrontFace frontFace;
		UInt32 lineWidth;

		UInt32 multisamples;

		Bool8 enableDepthTest;
		Bool8 enableDepthWrite;
		Bool8 enableDepthBoundsTest;
		Bool8 enableStencilTest;
		VkCompareOp depthCompareOp;
		VkStencilOp frontFailOp;
		VkStencilOp frontPassOp;
		VkStencilOp frontDepthFailOp;
		VkCompareOp frontCompareOp;
		UInt32 frontCompareMask;
		UInt32 frontWriteMask;
		UInt32 frontReference;
		VkStencilOp backFailOp;
		VkStencilOp backPassOp;
		VkStencilOp backDepthFailOp;
		VkCompareOp backCompareOp;
		UInt32 backCompareMask;
		UInt32 backWriteMask;
		UInt32 backReference;
		Float32 minDepth;
		Float32 maxDepth;

		Bool8 enableColorBlendLogicOp;
		VkLogicOp colorBlendLogicOp;
		Array<VulkanBlendAttachment> colorBlendAttachments;

		VulkanPipelineLayout pipelineLayout;

		VulkanRenderPass renderPass;
		UInt32 subpassIndex;
	};

	class QUARTZ_API VulkanGraphicsPipeline : public GFXGraphicsPipeline
	{
	private:
		VulkanGraphicsPipelineState	mPipelineState;
		VulkanDevice*				mpParentDevice;
		VkPipeline					mPipeline;
		Bool8						mIsValidPipeline;

	public:
		VulkanGraphicsPipeline() = default;
		VulkanGraphicsPipeline(VulkanDevice& device, const VulkanGraphicsPipelineState& pipelineState, const VulkanGraphicsPipeline* pParentPipeline);
	
		const VulkanGraphicsPipelineState& GetPipelineState() const { return mPipelineState; }
		VulkanDevice& GetParentDevice() { return *mpParentDevice; }
		VkPipeline GetPipelineHandle() const { return mPipeline; }

		Bool8 IsValidPipeline() const { return mIsValidPipeline; }

		void Destroy();
	};
}