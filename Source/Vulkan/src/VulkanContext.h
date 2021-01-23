#pragma once
#include "Common.h"

#include "util/Array.h"

#include "graphics/VGFXContext.h"
#include "SPIRVUtil.h"

#include "VulkanPhysicalDevice.h"
#include "VulkanDevice.h"

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
		Int32					frameIndex;

		Array<VkSemaphore>		imageAcquiredSemaphores;
		Array<VkSemaphore>		imageCompleteSemaphores;
		Array<VkFence>			allFences;
		Array<VkFence>			imageFenceMap;

		UInt32 width;
		UInt32 height;
	};

	struct VulkanShader : public VGFXShader
	{
		VkShaderModule			vkShader;
		VkPipelineStageFlags	stage;
		String					entryPoint;
		Array<SpirvUniform>		uniforms;
		Array<SpirvAttribute>	attributes;
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

	struct VulkanRenderPass : public VGFXRenderPass
	{
		VkRenderPass vkRenderPass;
	};

	struct VulkanFramebuffer : public VGFXFramebuffer
	{
		VkFramebuffer vkFramebuffer;
		UInt32 width;
		UInt32 height;
	};

	struct VulkanGraphicsPipeline : public VGFXGraphicsPipeline
	{
		VkPipeline					vkPipeline;
		VulkanGraphicsPipelineInfo	pipelineInfo;
		Array<VkDescriptorSet>		descriptorSets;
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

		//@TODO: Get rid of this
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
		GFXCommnadBufferUsages	usage;
	};

	struct VulkanBuffer : public VGFXBuffer
	{
		VkBuffer vkBuffer;
		VulkanDeviceMemoryAllocation* pMemory;
		UInt32 sizeBytes;
	};

	struct VulkanSampler : public VGFXSampler
	{
		VkSampler vkSampler;
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

		UInt32							mBackbufferCount;

	protected:
		Bool8 InitInstance(const StringW& appName, const Array<String>& extensions,
			const Array<String>& validationLayers);
		Bool8 InitDevices();

		void CreateImageImpl(VkImage* pVkImage, VulkanDeviceMemoryAllocation* pMemory, 
			VkImageType vkImageType, VkImageUsageFlags vkUsageFlags, VkFormat vkFormat,
			UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, UInt32 layers);

		void CreateImageViewImpl(VkImageView* pVkImageView, VkImage vkImage, 
			VkImageViewType vkImageViewType, VkImageAspectFlags vkAspectFlags, VkFormat vkFormat,
			UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layers);

		void CreateDescriptorSetLayoutImpl(VkDescriptorSetLayout* pVkDescriptorSetLayout, 
			const Array<VkDescriptorSetLayoutBinding>& bindings);

		void CreatePipelineLayoutImpl(VkPipelineLayout* pVkPipelineLayout,
			const Array<VkDescriptorSetLayout>& desciptorSets);

		void CreateGraphicsPipelineImpl(VkPipeline* pPipeline, const VulkanGraphicsPipelineInfo& info, VkPipeline parentId);

		void GenerateDescriptorSets(Array<VkDescriptorSetLayout>& descriptorSetLayouts, const Array<VulkanShader>& shaders);

	public:
		VPLVulkanContext();

		void PreInitialize() override;

		void Initialize() override;

		HGFXImage CreateImage(GFXImageType type, GFXImageUsages usages, GFXImageFormat imageFormat,
			UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, UInt32 layers) override;
		void DestroyImage(HGFXImage image) override;

		HGFXImageView CreateImageView(HGFXImage image, GFXImageViewType viewType, GFXImageUsage usage,
			UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layers) override;
		void DestroyImageView(HGFXImageView imageView) override;

		HGFXRenderPass CreateRenderPass(const GFXRenderPassInfo& renderPassInfo) override;

		HGFXFramebuffer CreateFramebuffer(HGFXRenderPass renderPass, const Array<HGFXImageView> imageViews, UInt32 width, UInt32 height) override;

		HGFXGraphicsPipeline CreateGraphicsPipeline(const GFXGraphicsPipelineInfo& pipelineInfo, 
			HGFXRenderPass renderPass, UInt32 renderPassIndex) override;
	
		HGFXCommandBuffer CreateCommandBuffer(GFXCommnadBufferUsages usage) override;

		HGFXShader CreateShader(GFXShaderStage shaderStage, const Array<Byte>& shaderData, const char* entryPoint) override;
	
		HGFXBuffer CreateBuffer(GFXBufferUsages usages, GFXBufferAccess access, UInt32 sizeBytes) override;

		HGFXUniform CreateUniform(GFXUniformType uniformType, HGFXBuffer buffer, UInt32 offset) override;

		HGFXSampler CreateSampler(GFXSamplerFilter filter, GFXSamplerFilter mipmapFilter, GFXSamplerMode mode, UInt32 anisotropy) override;

		void* MapBuffer(HGFXBuffer buffer) override;

		void UnmapBuffer(HGFXBuffer buffer) override;

		void CopyBuffer(HGFXBuffer source, HGFXBuffer dest) override;

		void CopyBufferToImage(HGFXBuffer buffer, HGFXImage image) override;

		void TransitionImage(HGFXImage image, GFXImageLayout oldLayout, GFXImageLayout newLayout) override;

		void BeginCommandBuffer(HGFXCommandBuffer commandBuffer) override;

		void EndCommandBuffer(HGFXCommandBuffer commandBuffer) override;

		void BeginRenderPass(HGFXCommandBuffer commandBuffer, HGFXRenderPass renderPass, HGFXFramebuffer framebuffer) override;

		void EndRenderPass(HGFXCommandBuffer commandBuffer) override;

		void BindGraphicsPipeline(HGFXCommandBuffer commandBuffer, HGFXGraphicsPipeline pipeline, UInt32 bufferIndex) override;

		void BindVertexBuffers(HGFXCommandBuffer commandBuffer, const Array<HGFXBuffer>& buffers) override;

		void BindIndexBuffer(HGFXCommandBuffer commandBuffer, HGFXBuffer buffer) override;

		void DrawIndexed(HGFXCommandBuffer commandBuffer, UInt32 count, UInt32 start) override;

		void Submit(HGFXCommandBuffer commandBuffer, HGFXSwapchain swapchain) override;

		void Present(HGFXSwapchain swapchain) override;

		void SetUniformBuffer(HGFXGraphicsPipeline pipeline, UInt32 set, UInt32 binding, HGFXBuffer buffer, UInt32 bufferIndex) override;

		void SetUniformSampledImage(HGFXGraphicsPipeline pipeline, UInt32 set, UInt32 binding, 
			HGFXSampler sampler, HGFXImageView imageView, UInt32 bufferIndex) override;

		HGFXImageView GetSwapchainImageView(HGFXSwapchain swapchain, UInt32 imageIndex) override;

		UInt32 AcquireSwapchainImageIndex(HGFXSwapchain swapchain) override;
	};
}