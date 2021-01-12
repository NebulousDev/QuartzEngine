#pragma once

#include "Common.h"
#include "util/Array.h"

#include "../platform/VPWindow.h"

namespace Quartz
{
	/* Virtual Graphics Objects */

	struct VGFXResource {};

	struct VGFXGraphicsPipeline : public VGFXResource {};
	struct VGFXComputePipeline : public VGFXResource {};
	struct VGFXRenderAttachment : public VGFXResource {};
	struct VGFXRenderPass : public VGFXResource {};
	struct VGFXBuffer : public VGFXResource {};
	struct VGFXImage : public VGFXResource {};
	struct VGFXImageView : public VGFXResource {};
	struct VGFXSurface : public VGFXResource {};
	struct VGFXSwapchain : public VGFXResource {};
	struct VGFXCommandBuffer : public VGFXResource {};
	struct VGFXFramebuffer : public VGFXResource {};
	struct VGFXShader : public VGFXResource {};
	struct VGFXUniform : public VGFXResource {};

	/* Virtual Graphics Handles */

	typedef VGFXResource* HGFXGraphicsPipeline;
	typedef VGFXResource* HGFXComputePipeline;
	typedef VGFXResource* HGFXRenderAttachment;
	typedef VGFXResource* HGFXRenderPass;
	typedef VGFXResource* HGFXBuffer;
	typedef VGFXResource* HGFXImage;
	typedef VGFXResource* HGFXImageView;
	typedef VGFXResource* HGFXSurface;
	typedef VGFXResource* HGFXSwapchain;
	typedef VGFXResource* HGFXCommandBuffer;
	typedef VGFXResource* HGFXFramebuffer;
	typedef VGFXResource* HGFXShader;
	typedef VGFXResource* HGFXUniform;

	/* Null Handles */

#define HGFX_NULL_HANDLE (VGFXResource*)nullptr

	/* Graphics Object Initialization Structures */

	struct GFXViewport
	{
		Bounds2f bounds;
		Float32 minDepth;
		Float32 maxDepth;
	};

	struct GFXScissor
	{
		Bounds2f bounds;
	};

	struct GFXColorBlend
	{
		//TODO: Implement
	};

	struct GFXBlendAttachment
	{
		GFXColorBlend blend;
	};

	enum GFXPrimitiveTopology
	{
		GFX_PRIMITIVE_TOPOLOGY_POINTS,
		GFX_PRIMITIVE_TOPOLOGY_LINES,
		GFX_PRIMITIVE_TOPOLOGY_TRIANGLES
	};

	enum GFXPolygonMode
	{
		GFX_POLYGON_MODE_FILL,
		GFX_POLYGON_MODE_LINE,
		GFX_POLYGON_MODE_POINT,
	};

	enum GFXCullMode
	{
		GFX_CULL_MODE_NONE,
		GFX_CULL_MODE_FRONT,
		GFX_CULL_MODE_BACK,
		GFX_CULL_MODE_FRONT_AND_BACK
	};

	enum GFXFaceWind
	{
		GFX_FACE_WIND_COUNTER_CLOCKWISE,
		GFX_FACE_WIND_CLOCKWISE
	};

	enum GFXMultisample
	{
		GFX_MULTISAMPLE_DISABLED,
		GFX_MULTISAMPLE_2X,
		GFX_MULTISAMPLE_4X,
		GFX_MULTISAMPLE_8X
	};

	enum GFXCompareOperation
	{
		GFX_COMPARE_OP_NONE,
		GFX_COMPARE_OP_LESS,
		GFX_COMPARE_OP_EQUAL,
		GFX_COMPARE_OP_LESS_OR_EQUAL,
		GFX_COMPARE_OP_GREATER,
		GFX_COMPARE_OP_NOT_EQUAL,
		GFX_COMPARE_OP_GREATER_OR_EQUAL,
		GFX_COMPARE_OP_ALWAYS,
	};

	struct GFXBufferAttachent
	{
		UInt32 binding;
		UInt32 stride;
	};

	enum GFXAttributeType
	{
		GFX_ATTRIBUTE_TYPE_FLOAT,
		GFX_ATTRIBUTE_TYPE_FLOAT2,
		GFX_ATTRIBUTE_TYPE_FLOAT3,
		GFX_ATTRIBUTE_TYPE_FLOAT4,
		GFX_ATTRIBUTE_TYPE_INT,
		GFX_ATTRIBUTE_TYPE_INT2,
		GFX_ATTRIBUTE_TYPE_INT3,
		GFX_ATTRIBUTE_TYPE_INT4,
		GFX_ATTRIBUTE_TYPE_UINT,
		GFX_ATTRIBUTE_TYPE_UINT2,
		GFX_ATTRIBUTE_TYPE_UINT3,
		GFX_ATTRIBUTE_TYPE_UINT4,
		GFX_ATTRIBUTE_TYPE_INT_2_10_10_10,
		GFX_ATTRIBUTE_TYPE_UINT_2_10_10_10
	};

	struct GFXVertexAttribute
	{
		UInt32 binding;
		UInt32 location;
		GFXAttributeType type;
	};

	enum GFXDescriptorType
	{
		GFX_DESCRIPTOR_TYPE_UNIFORM,
		GFX_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
	};

	enum GFXShaderStage
	{
		GFX_SHADER_STAGE_VERTEX,
		GFX_SHADER_STAGE_PIXEL,
		GFX_SHADER_STAGE_HULL,
		GFX_SHADER_STAGE_DOMAIN,
		GFX_SHADER_STAGE_GEOMETRY,
		GFX_SHADER_STAGE_TASK,
		GFX_SHADER_STAGE_MESH
	};

	enum GFXShaderStageBits
	{
		GFX_SHADER_STAGE_VERTEX_BIT		= 0x01,
		GFX_SHADER_STAGE_PIXEL_BIT		= 0x02,
		GFX_SHADER_STAGE_HULL_BIT		= 0x04,
		GFX_SHADER_STAGE_DOMAIN_BIT		= 0x08,
		GFX_SHADER_STAGE_GEOMETRY_BIT	= 0x10,
		GFX_SHADER_STAGE_TASK_BIT		= 0x20,
		GFX_SHADER_STAGE_MESH_BIT		= 0x40
	};

	typedef Flags32 GFXShaderStages;

	struct GFXDescriptorAttachment
	{
		UInt32 set;
		UInt32 binding;
		GFXDescriptorType type;
		UInt32 arraySize;
		GFXShaderStages stages;
	};

	enum GFXLoadOperation
	{
		GFX_LOAD_OP_DONT_CARE,
		GFX_LOAD_OP_LOAD,
		GFX_LOAD_OP_CLEAR
	};

	enum GFXStoreOperation
	{
		GFX_STORE_OP_DONT_CARE,
		GFX_STORE_OP_STORE
	};

	enum GFXImageLayout
	{
		GFX_IMAGE_LAYOUT_UNDEFINED,
		GFX_IMAGE_LAYOUT_COLOR_OUTPUT,
		GFX_IMAGE_LAYOUT_DEPTH_OUTPUT,
		GFX_IMAGE_LAYOUT_STENCIL_OUTPUT,
		GFX_IMAGE_LAYOUT_DEPTH_STENCIL_OUTPUT,
		GFX_IMAGE_LAYOUT_COLOR_INPUT,
		GFX_IMAGE_LAYOUT_DEPTH_INPUT,
		GFX_IMAGE_LAYOUT_STENCIL_INPUT,
		GFX_IMAGE_LAYOUT_DEPTH_STENCIL_INPUT,
		GFX_IMAGE_LAYOUT_TRANSFER_SOURCE,
		GFX_IMAGE_LAYOUT_TRANSFER_DESTINATION,
		GFX_IMAGE_LAYOUT_PRESENT
	};

	enum GFXImageFormat
	{
		GFX_IMAGE_FORMAT_UNDEFINED,
		GFX_IMAGE_FORMAT_R8_UNORM,
		GFX_IMAGE_FORMAT_RG8_UNORM,
		GFX_IMAGE_FORMAT_RGB8_UNORM,
		GFX_IMAGE_FORMAT_RGBA8_UNORM,
		GFX_IMAGE_FORMAT_BGRA8_UNORM,
		GFX_IMAGE_FORMAT_DEPTH24_UNORM_STENCIL8_UINT
	};

	struct GFXRenderAttachment
	{
		GFXImageFormat format;
		GFXLoadOperation loadOp;
		GFXStoreOperation storeOp;
		GFXLoadOperation stencilLoadOp;
		GFXStoreOperation stencilStoreOp;
		GFXImageLayout initalLayout;
		GFXImageLayout finalLayout;
	};

	struct GFXRenderSubpassAttachment
	{
		GFXRenderAttachment* pAttachment;
		GFXImageLayout layout;
	};

	enum GFXSubpassIndex
	{
		GFX_SUBPASS_EXTERNAL,
		GFX_SUBPASS_INDEX_0,
		GFX_SUBPASS_INDEX_1,
		GFX_SUBPASS_INDEX_2,
		GFX_SUBPASS_INDEX_3,
		GFX_SUBPASS_INDEX_4,
		GFX_SUBPASS_INDEX_5,
		GFX_SUBPASS_INDEX_6,
		GFX_SUBPASS_INDEX_7
	};

	enum GFXPipelineStageBits
	{
		GFX_PIPELINE_STAGE_NONE = 0x000,
		GFX_PIPELINE_STAGE_TOP_OF_PIPE_BIT = 0x001,
		GFX_PIPELINE_STAGE_VERTEX_INPUT_BIT = 0x002,
		GFX_PIPELINE_STAGE_VERTEX_SHADER_BIT = 0x004,
		GFX_PIPELINE_STAGE_HULL_SHADER_BIT = 0x008,
		GFX_PIPELINE_STAGE_DOMAIN_SHADER_BIT = 0x010,
		GFX_PIPELINE_STAGE_GEOMETRY_SHADER_BIT = 0x020,
		GFX_PIPELINE_STAGE_PIXEL_SHADER_BIT = 0x040,
		GFX_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT = 0x080,
		GFX_PIPELINE_STAGE_COMPUTE_SHADER_BIT = 0x100,
		GFX_PIPELINE_STAGE_TRANSFER_BIT = 0x200,
		GFX_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT = 0x400,
		GFX_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT = 0x800
	};

	typedef Flags32 GFXPipelineStages;

	enum GFXPipelineAccessBits
	{
		GFX_ACCESS_INDIRECT_COMMAND_READ_BIT = 0x00001,
		GFX_ACCESS_INDEX_READ_BIT = 0x00002,
		GFX_ACCESS_VERTEX_ATTRIBUTE_READ_BIT = 0x00004,
		GFX_ACCESS_UNIFORM_READ_BIT = 0x00008,
		GFX_ACCESS_INPUT_ATTACHMENT_READ_BIT = 0x00010,
		GFX_ACCESS_SHADER_READ_BIT = 0x00020,
		GFX_ACCESS_SHADER_WRITE_BIT = 0x00040,
		GFX_ACCESS_COLOR_ATTACHMENT_READ_BIT = 0x00080,
		GFX_ACCESS_COLOR_ATTACHMENT_WRITE_BIT = 0x00100,
		GFX_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT = 0x00200,
		GFX_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT = 0x00400,
		GFX_ACCESS_TRANSFER_READ_BIT = 0x00800,
		GFX_ACCESS_TRANSFER_WRITE_BIT = 0x01000,
		GFX_ACCESS_HOST_READ_BIT = 0x02000,
		GFX_ACCESS_HOST_WRITE_BIT = 0x04000,
		GFX_ACCESS_MEMORY_READ_BIT = 0x08000,
		GFX_ACCESS_MEMORY_WRITE_BIT = 0x10000,
	};

	typedef Flags32 GFXPipelineAccess;

	struct GFXSubpassDependancy
	{
		GFXSubpassIndex subpassSrcIndex;
		GFXPipelineStages srcStage;
		GFXPipelineAccess srcAccess;

		GFXSubpassIndex subpassDestIndex;
		GFXPipelineStages destStage;
		GFXPipelineAccess destAccess;
	};

	struct GFXRenderSubpassInfo
	{
		const char* subpassName;
		Array<GFXRenderSubpassAttachment> attachments;
	};

	struct GFXRenderPassInfo
	{
		Array<GFXRenderSubpassInfo> subpasses;
		Array<GFXSubpassDependancy> dependancies;
	};

	struct GFXGraphicsPipelineInfo
	{
		HGFXShader	vertexShader;
		HGFXShader	pixelShader;
		HGFXShader	geometryShader;

		Array<GFXBufferAttachent> bufferAttachemnts;
		Array<GFXVertexAttribute> vertexAttributes;
		//Array<GFXDescriptorAttachment> descriptorAttachents;
		Array<GFXBlendAttachment> blendAttachments;

		GFXViewport viewport;
		GFXScissor scissor;

		GFXPrimitiveTopology topology;

		GFXPolygonMode	polygonMode;
		GFXCullMode cullMode;
		GFXFaceWind faceWind;
		Float32 lineWidth;

		GFXMultisample multisample;

		Bool8 depthTesting;
		GFXCompareOperation depthOperation;

		// @Todo this should be moved
		UInt32 backbufferCount;
	};

	enum GFXImageType
	{
		GFX_IMAGE_TYPE_1D,
		GFX_IMAGE_TYPE_2D,
		GFX_IMAGE_TYPE_3D
	};

	enum GFXImageViewType
	{
		GFX_IMAGE_VIEW_TYPE_1D,
		GFX_IMAGE_VIEW_TYPE_2D,
		GFX_IMAGE_VIEW_TYPE_3D,
		GFX_IMAGE_VIEW_TYPE_CUBEMAP,
		GFX_IMAGE_VIEW_TYPE_ARRAY_1D,
		GFX_IMAGE_VIEW_TYPE_ARRAY_2D
	};

	enum GFXImageUsage
	{
		GFX_IMAGE_USAGE_INVALID,
		GFX_IMAGE_USAGE_SAMPLED_TEXTURE,
		GFX_IMAGE_USAGE_COLOR_ATTACHMENT,
		GFX_IMAGE_USAGE_DEPTH_ATTACHMENT,
		GFX_IMAGE_USAGE_STENCIL_ATTACHMENT,
		GFX_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT
	};

	enum GFXImageUsageBits
	{
		GFX_IMAGE_USAGE_SAMPLED_TEXTURE_BIT				= 0x01,
		GFX_IMAGE_USAGE_COLOR_ATTACHMENT_BIT			= 0x02,
		GFX_IMAGE_USAGE_DEPTH_ATTACHMENT_BIT			= 0x04,
		GFX_IMAGE_USAGE_STENCIL_ATTACHMENT_BIT			= 0x08,
		GFX_IMAGE_USAGE_INPUT_ATTACHMENT_BIT			= 0x10,
		GFX_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT	= GFX_IMAGE_USAGE_DEPTH_ATTACHMENT_BIT | GFX_IMAGE_USAGE_STENCIL_ATTACHMENT_BIT,
		GFX_IMAGE_USAGE_TRANSFER_SRC_BIT				= 0x20,
		GFX_IMAGE_USAGE_TRANSFER_DST_BIT				= 0x40
	};

	typedef Flags32 GFXImageUsages;

	enum GFXCommandBufferUsageBits
	{
		GFX_COMMAND_BUFFER_USAGE_GRAPHICS_BIT	= 0x1,
		GFX_COMMAND_BUFFER_USAGE_COMPUTE_BIT	= 0x2,
		GFX_COMMAND_BUFFER_USAGE_TRANSFER_BIT	= 0x4
	};

	typedef Flags32 GFXCommnadBufferUsages;

	enum GFXBufferUsageBits
	{
		GFX_BUFFER_USAGE_TRANSFER_SRC_BIT		= 0x01,
		GFX_BUFFER_USAGE_TRANSFER_DEST_BIT		= 0x02,
		GFX_BUFFER_USAGE_VERTEX_BUFFER_BIT		= 0x04,
		GFX_BUFFER_USAGE_INDEX_BUFFER_BIT		= 0x08,
		GFX_BUFFER_USAGE_UNIFORM_BUFFER_BIT		= 0x10,
		GFX_BUFFER_USAGE_STORAGE_BUFFER_BIT		= 0x20
	};

	typedef Flags32 GFXBufferUsages;

	enum GFXBufferAccessBits
	{
		GFX_BUFFER_ACCESS_DEVICE_LOCAL_BIT	= 0x01,
		GFX_BUFFER_ACCESS_HOST_VISIBLE_BIT	= 0x02,
		GFX_BUFFER_ACCESS_HOST_COHERENT_BIT = 0x04,
		GFX_BUFFER_ACCESS_HOST_CACHED_BIT	= 0x08,
	};

	typedef Flags32 GFXBufferAccess;

	enum GFXUniformType
	{
		GFX_UNIFORM_TYPE_STRUCT
	};

	/* The virtual graphics context */

	class QUARTZ_API VGFXContext
	{
	public:
		virtual void PreInitialize() = 0;

		virtual void Initialize() = 0;

		virtual HGFXSurface CreateSurface(HVPWindow window, UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen) = 0;
		virtual void DestroySurface(HGFXSurface surface) = 0;

		virtual HGFXSwapchain CreateSwapchain(HGFXSurface surface, UInt32 bufferCount,
			UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen) = 0;
		virtual void DestroySwapchain(HGFXSwapchain swapchain) = 0;

		virtual HGFXImage CreateImage(GFXImageType type, GFXImageUsages usages, GFXImageFormat imageFormat,
			UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, UInt32 layers) = 0;
		virtual void DestroyImage(HGFXImage image) = 0;

		virtual HGFXImageView CreateImageView(HGFXImage image, GFXImageViewType viewType, GFXImageUsage usage,
			UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layers) = 0;
		virtual void DestroyImageView(HGFXImageView imageView) = 0;

		virtual HGFXRenderPass CreateRenderPass(const GFXRenderPassInfo& renderPassInfo) = 0;

		virtual HGFXFramebuffer CreateFramebuffer(HGFXRenderPass renderPass, const Array<HGFXImageView> imageViews, UInt32 width, UInt32 height) = 0;

		virtual HGFXGraphicsPipeline CreateGraphicsPipeline(const GFXGraphicsPipelineInfo& pipelineInfo, 
			HGFXRenderPass renderPass, UInt32 renderPassIndex) = 0;

		virtual HGFXCommandBuffer CreateCommandBuffer(GFXCommnadBufferUsages usage) = 0;

		virtual HGFXShader CreateShader(GFXShaderStage shaderStage, const Array<Byte>& shaderData, const char* entryPoint) = 0;
	
		virtual HGFXBuffer CreateBuffer(GFXBufferUsages usages, GFXBufferAccess access, UInt32 sizeBytes) = 0;

		virtual HGFXUniform CreateUniform(GFXUniformType uniformType, HGFXBuffer buffer, UInt32 offset) = 0;

		virtual void* MapBuffer(HGFXBuffer buffer) = 0;

		virtual void UnmapBuffer(HGFXBuffer buffer) = 0;

		virtual void BeginCommandBuffer(HGFXCommandBuffer commandBuffer) = 0;

		virtual void EndCommandBuffer(HGFXCommandBuffer commandBuffer) = 0;

		virtual void BeginRenderPass(HGFXCommandBuffer commandBuffer, HGFXRenderPass renderPass, HGFXFramebuffer framebuffer) = 0;

		virtual void EndRenderPass(HGFXCommandBuffer commandBuffer) = 0;

		// @Todo: Get rid of buffer index
		virtual void BindGraphicsPipeline(HGFXCommandBuffer commandBuffer, HGFXGraphicsPipeline pipeline, UInt32 bufferIndex) = 0;

		virtual void BindVertexBuffers(HGFXCommandBuffer commandBuffer, const Array<HGFXBuffer>& buffers) = 0;

		virtual void BindIndexBuffer(HGFXCommandBuffer commandBuffer, HGFXBuffer buffer) = 0;

		virtual void DrawIndexed(HGFXCommandBuffer commandBuffer, UInt32 count, UInt32 start) = 0;

		virtual void Submit(HGFXCommandBuffer commandBuffer, HGFXSwapchain swapchain) = 0;

		virtual void Present(HGFXSwapchain swapchain) = 0;

		// @Todo: This is awful. Please properly build a system for this
		virtual void SetUniformBuffer(HGFXGraphicsPipeline pipeline, UInt32 set, UInt32 binding, HGFXBuffer buffer, UInt32 bufferIndex) = 0;

		virtual UInt32 AcquireSwapchainImageIndex(HGFXSwapchain swapchain) = 0;

		virtual HGFXImageView GetSwapchainImageView(HGFXSwapchain swapchain, UInt32 imageIndex) = 0;
	};
}