#pragma once
#include "Common.h"

#include "util/Array.h"

#include "graphics2/VGFXContext.h"
#include "../VulkanPhysicalDevice.h"
#include "../VulkanDevice.h"

namespace Quartz
{
	struct VulkanImage;
	struct VulkanImageView;

	struct VulkanSurface : public VGFXSurface
	{
		VkSurfaceKHR	surface;
		Bounds2i		bounds;
	};

	struct VulkanSwapchain : public VGFXSwapchain
	{
		VkSwapchainKHR			vkSwapchain;
		VkFormat				vkFormat;
		Array<VulkanImage>		images;
		Array<VulkanImageView>	imageViews;
		UInt32					imageCount;
		Int32					imageIndex;

		Array<VkSemaphore>		imageAcquiredSemaphores;
		Array<VkSemaphore>		imageCompleteSemaphores;
		Array<VkFence>			imageFences;

		UInt32 width;
		UInt32 height;
	};

	struct VulkanShader : public VGFXShader
	{
		VkShaderModule	vkShader;
		GFXShaderStage	stage;
		String			entryPoint;
	};

	enum GFXVertexElementAttribute
	{
		GFX_VERTEX_ATTRIBUTE_UNKNOWN,
		GFX_VERTEX_ATTRIBUTE_POSITION,
		GFX_VERTEX_ATTRIBUTE_NORMAL,
		GFX_VERTEX_ATTRIBUTE_BINORMAL,
		GFX_VERTEX_ATTRIBUTE_TANGENT,
		GFX_VERTEX_ATTRIBUTE_TEXCOORD,
		GFX_VERTEX_ATTRIBUTE_COLOR,
		GFX_VERTEX_ATTRIBURE_CUSTOM
	};

	enum GFXVertexElementType
	{
		GFX_VERTEX_TYPE_FLOAT,
		GFX_VERTEX_TYPE_FLOAT2,
		GFX_VERTEX_TYPE_FLOAT3,
		GFX_VERTEX_TYPE_FLOAT4,
		GFX_VERTEX_TYPE_INT,
		GFX_VERTEX_TYPE_INT2,
		GFX_VERTEX_TYPE_INT3,
		GFX_VERTEX_TYPE_INT4,
		GFX_VERTEX_TYPE_UINT,
		GFX_VERTEX_TYPE_UINT2,
		GFX_VERTEX_TYPE_UINT3,
		GFX_VERTEX_TYPE_UINT4,
		GFX_VERTEX_TYPE_INT_2_10_10_10,
		GFX_VERTEX_TYPE_UINT_2_10_10_10
	};

	struct VulkanVertexBinding
	{
		UInt32				binding;
		UInt32				stride;
		VkVertexInputRate	inputRate;
	};

	struct VulkanVertexAttribute
	{
		UInt32		binding;
		UInt32		location;
		VkFormat	format;
		UInt32		offset;
	};

	struct VulkanBlendAttachment
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

	struct VulkanDescriptorSetLayoutBinding
	{
		UInt32 binding;
		VkDescriptorType descriptorType;
		UInt32 descriptorCount; //NOTE: uniform array size
		VkShaderStageFlags stageFlags;

		// Potentially Array<...>?
		const VkSampler* pImmutableSamplers;
	};

	struct VulkanGraphicsPipelineInfo
	{
		VkPipelineCreateFlags flags;

		VulkanShader vertexShader;
		VulkanShader hullShader;
		VulkanShader domainShader;
		VulkanShader geometryShader;
		VulkanShader pixelShader;

		Array<VulkanVertexBinding> vertexBindings;
		Array<VulkanVertexAttribute> vertexAttributes;

		VkPrimitiveTopology topology;

		Array<VkViewport> viewports;
		Array<VkRect2D> scissors;

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

		VkPipelineLayout pipelineLayout;

		VkRenderPass renderPass;
		UInt32 subpassIndex;
	};

	struct VulkanGraphicsPipeline : public VGFXGraphicsPipeline
	{
		VkPipeline					vkPipeline;
		VulkanGraphicsPipelineInfo	pipelineInfo;
	};

	struct VulkanImageView : public VGFXImageView
	{
		VkImageView			vkImageView;
		VkFormat			vkFormat;
		VkImageViewType		vkViewType;
		VkImageAspectFlags	vkAspects;

		UInt32 mipStart;
		UInt32 mipLevels;
		UInt32 layerStart;
		UInt32 layers;
	};

	struct VulkanImage : public VGFXImage
	{
		VkImage				vkImage;
		VkFormat			vkFormat;
		VkImageUsageFlags	vkUsage;
		VkImageType			vkImageType;

		VulkanDeviceMemoryAllocation*	pMemory;

		UInt32 width;
		UInt32 height;
		UInt32 depth;
		UInt32 mipLevels;
		UInt32 layers;
	};

	struct VulkanCommandBuffer : public VGFXCommandBuffer
	{
		VkCommandBuffer vkCommandBuffer;
		VkCommandPool	vkCommandPool;
		GFXBufferUsages	usage;
	};

	class QUARTZ_API VPLVulkanContext : public VGFXContext
	{
	protected:
		VkInstance						mVkInstance;
		Array<VulkanPhysicalDevice>		mPhysicalDevices;
		VulkanDevice*					mpDevice;
		Array<VkLayerProperties>		mAvailableLayers;
		Array<VkExtensionProperties>	mAvailableExtensions;
		Array<VkPhysicalDevice>			mAvailablePhysicalDevices;
		Array<const char*>				mEnabledLayerNames;
		Array<const char*>				mEnabledExtensionNames;

	protected:
		Bool8 InitInstance(const String& appName, const Array<String>& extensions,
			const Array<String>& validationLayers);
		Bool8 InitDevices();

		void CreateImageImpl(VkImage* pVkImage, VulkanDeviceMemoryAllocation* pMemory, 
			VkImageType vkImageType, VkImageUsageFlags vkUsageFlags, VkFormat vkFormat,
			UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, UInt32 layers);

		void CreateImageViewImpl(VkImageView* pVkImageView, VkImage vkImage, 
			VkImageViewType vkImageViewType, VkImageAspectFlags vkAspectFlags, VkFormat vkFormat,
			UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layers);

		void CreateDiscriptorSetLayoutImpl(VkDescriptorSetLayout* pVkDescriptorSetLayout, 
			const Array<VkDescriptorSetLayoutBinding>& bindings);

		void CreatePipelineLayoutImpl(VkPipelineLayout* pVkPipelineLayout,
			const Array<VkDescriptorSetLayout>& desciptorSets);

		void CreateGraphicsPipelineImpl(VkPipeline* pPipeline, const VulkanGraphicsPipelineInfo& info, VkPipeline parent);

	public:
		VPLVulkanContext();

		void InitInstanceAndDevices(const String& appName, const Array<String>& extensions, 
			const Array<String>& validationLayers);

		HGFXImage CreateImage(GFXImageType type, GFXImageUsages usages, GFXImageFormat imageFormat,
			UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, UInt32 layers) override;

		HGFXImageView CreateImageView(HGFXImage image, GFXImageViewType viewType, GFXImageUsage usage,
			UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layers) override;

		HGFXRenderPass CreateRenderPass(const GFXRenderPassInfo& renderPassInfo) override;

		HGFXGraphicsPipeline CreateGraphicsPipeline(const GFXGraphicsPipelineInfo& pipelineInfo) override;
	
		HGFXCommandBuffer CreateCommandBuffer(GFXBufferUsages usage) override;

		HGFXShader CreateShader(GFXShaderStage shaderStage, const Array<Byte>& shaderData, const char* entryPoint) override;
	};
}