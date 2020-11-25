#pragma once

#include "graphics2/VGFXContext.h"
#include "vulkan/vulkan.h"

namespace Quartz
{
	FORCE_INLINE VkFormat ImageFormatToVkFormat(GFXImageFormat format)
	{
		static VkFormat sFormatTable[] =
		{
			VK_FORMAT_UNDEFINED,
			VK_FORMAT_R8_UNORM,
			VK_FORMAT_R8G8_UNORM,
			VK_FORMAT_R8G8B8_UNORM,
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_FORMAT_B8G8R8A8_UNORM,
			VK_FORMAT_D24_UNORM_S8_UINT
		};

		return sFormatTable[(UInt32)format];
	}

	FORCE_INLINE VkImageType ImageTypeToVkImageType(GFXImageType type)
	{
		switch (type)
		{
			case GFX_IMAGE_TYPE_1D:		return VK_IMAGE_TYPE_1D;
			case GFX_IMAGE_TYPE_2D:		return VK_IMAGE_TYPE_2D;
			case GFX_IMAGE_TYPE_3D:		return VK_IMAGE_TYPE_3D;
			default:					return VK_IMAGE_TYPE_2D;
		}
	}

	FORCE_INLINE VkImageViewType ImageViewTypeToVkImageViewType(GFXImageViewType type)
	{
		switch (type)
		{
			case GFX_IMAGE_VIEW_TYPE_1D:		return VK_IMAGE_VIEW_TYPE_1D;
			case GFX_IMAGE_VIEW_TYPE_2D:		return VK_IMAGE_VIEW_TYPE_2D;
			case GFX_IMAGE_VIEW_TYPE_3D:		return VK_IMAGE_VIEW_TYPE_3D;
			case GFX_IMAGE_VIEW_TYPE_CUBEMAP:	return VK_IMAGE_VIEW_TYPE_CUBE;
			case GFX_IMAGE_VIEW_TYPE_ARRAY_1D:	return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
			case GFX_IMAGE_VIEW_TYPE_ARRAY_2D:	return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			default:							return VK_IMAGE_VIEW_TYPE_2D;
		}
	}

	FORCE_INLINE VkImageAspectFlags ImageUsageToVkImageAspects(GFXImageUsage usage)
	{
		static VkImageAspectFlags sAspectTable[] =
		{
			0,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT,
			VK_IMAGE_ASPECT_STENCIL_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT // TODO: look into this?
		};

		return sAspectTable[(UInt32)usage];
	}

	FORCE_INLINE VkImageLayout ImageLayoutToVkImageLayout(GFXImageLayout layout)
	{
		static VkImageLayout sLayoutTable[] =
		{
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
			VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL,
			VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL,
			VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
			VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
		};

		return sLayoutTable[(UInt32)layout];
	}

	FORCE_INLINE VkImageUsageFlags ImageUsagesToVkImageUsage(GFXImageUsages usages)
	{
		VkImageUsageFlags flags = 0;

		if ((usages & GFX_IMAGE_USAGE_SAMPLED_TEXTURE_BIT) == GFX_IMAGE_USAGE_SAMPLED_TEXTURE_BIT)
		{
			flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
		}

		if ((usages & GFX_IMAGE_USAGE_COLOR_ATTACHMENT_BIT) == GFX_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
		{
			flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}

		if ((usages & GFX_IMAGE_USAGE_DEPTH_ATTACHMENT_BIT) == GFX_IMAGE_USAGE_DEPTH_ATTACHMENT_BIT ||
			(usages & GFX_IMAGE_USAGE_STENCIL_ATTACHMENT_BIT) == GFX_IMAGE_USAGE_STENCIL_ATTACHMENT_BIT)
		{
			flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		}

		if ((usages & GFX_IMAGE_USAGE_INPUT_ATTACHMENT_BIT) == GFX_IMAGE_USAGE_INPUT_ATTACHMENT_BIT)
		{
			flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
		}

		if ((usages & GFX_IMAGE_USAGE_TRANSFER_SRC_BIT) == GFX_IMAGE_USAGE_TRANSFER_SRC_BIT)
		{
			flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		if ((usages & GFX_IMAGE_USAGE_TRANSFER_DST_BIT) == GFX_IMAGE_USAGE_TRANSFER_DST_BIT)
		{
			flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		return flags;
	}

	FORCE_INLINE VkAttachmentLoadOp LoadOperationToVkAttachmentLoadOp(GFXLoadOperation loadOp)
	{
		static VkAttachmentLoadOp sLoadOpTable[] =
		{
			VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			VK_ATTACHMENT_LOAD_OP_LOAD,
			VK_ATTACHMENT_LOAD_OP_CLEAR
		};

		return sLoadOpTable[(UInt32)loadOp];
	}

	FORCE_INLINE VkAttachmentStoreOp StoreOperationToVkAttachmentStoreOp(GFXStoreOperation storeOp)
	{
		static VkAttachmentStoreOp sStoreOpTable[] =
		{
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_STORE
		};

		return sStoreOpTable[(UInt32)storeOp];
	}

	FORCE_INLINE VkShaderStageFlags ShaderStageToVkShaderStageFlags(GFXShaderStage shaderStage)
	{
		static VkImageAspectFlags sShaderStageTable[] =
		{
			VK_SHADER_STAGE_VERTEX_BIT,
			VK_SHADER_STAGE_FRAGMENT_BIT,
			VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
			VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
			VK_SHADER_STAGE_GEOMETRY_BIT,
			VK_SHADER_STAGE_TASK_BIT_NV,
			VK_SHADER_STAGE_MESH_BIT_NV
		};

		return sShaderStageTable[(UInt32)shaderStage];
	}

	FORCE_INLINE UInt32 SubpassIndexToVkSubpassIndex(GFXSubpassIndex index)
	{
		static VkImageAspectFlags sSubpassIndex[] =
		{
			VK_SUBPASS_EXTERNAL,
			0, 1, 2, 3, 4, 5, 6, 7
		};

		return sSubpassIndex[(UInt32)index];
	}

	FORCE_INLINE VkPipelineStageFlags PipelineStageBitsToVkSubpassIndex(GFXPipelineStageBits index)
	{
		static VkPipelineStageFlags sSubpassIndex[] =
		{
			VK_SUBPASS_EXTERNAL,
			0, 1, 2, 3, 4, 5, 6, 7
		};

		return sSubpassIndex[(UInt32)index];
	}

	FORCE_INLINE UInt32 VertexElementSize(GFXVertexElementType elementType)
	{
		switch (elementType)
		{
			case GFX_VERTEX_TYPE_FLOAT:				return 1 * sizeof(Float32);
			case GFX_VERTEX_TYPE_FLOAT2:			return 2 * sizeof(Float32);
			case GFX_VERTEX_TYPE_FLOAT3:			return 3 * sizeof(Float32);
			case GFX_VERTEX_TYPE_FLOAT4:			return 4 * sizeof(Float32);
			case GFX_VERTEX_TYPE_INT:				return 1 * sizeof(Int32);
			case GFX_VERTEX_TYPE_INT2:				return 2 * sizeof(Int32);
			case GFX_VERTEX_TYPE_INT3:				return 3 * sizeof(Int32);
			case GFX_VERTEX_TYPE_INT4:				return 4 * sizeof(Int32);
			case GFX_VERTEX_TYPE_UINT:				return 1 * sizeof(UInt32);
			case GFX_VERTEX_TYPE_UINT2:				return 2 * sizeof(UInt32);
			case GFX_VERTEX_TYPE_UINT3:				return 3 * sizeof(UInt32);
			case GFX_VERTEX_TYPE_UINT4:				return 4 * sizeof(UInt32);
			case GFX_VERTEX_TYPE_INT_2_10_10_10:	return 1 * sizeof(Int32);
			case GFX_VERTEX_TYPE_UINT_2_10_10_10:	return 1 * sizeof(UInt32);
			default: return 0;
		}
	}

	//FORCE_INLINE UInt32 CalcVertexStride(GFXVertexFormat& vertexFormat)
	//{
	//	UInt32 stride = 0;
	//	for (UInt32 i = 0; i < vertexFormat.elementCount; i++)
	//	{
	//		stride += VertexElementSize(vertexFormat.elements[i].type);
	//	}

	//	return stride;
	//}

	FORCE_INLINE VkFormat VertexElementTypeToVkFormat(GFXVertexElementType elementType)
	{
		static VkFormat formatTable[] =
		{
			VK_FORMAT_R32_SFLOAT,
			VK_FORMAT_R32G32_SFLOAT,
			VK_FORMAT_R32G32B32_SFLOAT,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			VK_FORMAT_R32_SINT,
			VK_FORMAT_R32G32_SINT,
			VK_FORMAT_R32G32B32_SINT,
			VK_FORMAT_R32G32B32A32_SINT,
			VK_FORMAT_R32_UINT,
			VK_FORMAT_R32G32_UINT,
			VK_FORMAT_R32G32B32_UINT,
			VK_FORMAT_R32G32B32A32_UINT,
			VK_FORMAT_A2B10G10R10_SINT_PACK32,
			VK_FORMAT_A2B10G10R10_UINT_PACK32
		};

		return formatTable[(UInt32)elementType];
	}

	FORCE_INLINE VkFormat AttributeTypeToVkFormat(GFXAttributeType type)
	{
		static VkFormat typeTable[] =
		{
			VK_FORMAT_R32_SFLOAT,
			VK_FORMAT_R32G32_SFLOAT,
			VK_FORMAT_R32G32B32_SFLOAT,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			VK_FORMAT_R32_SINT,
			VK_FORMAT_R32G32_SINT,
			VK_FORMAT_R32G32B32_SINT,
			VK_FORMAT_R32G32B32A32_SINT,
			VK_FORMAT_R32_UINT,
			VK_FORMAT_R32G32_UINT,
			VK_FORMAT_R32G32B32_UINT,
			VK_FORMAT_R32G32B32A32_UINT,
			VK_FORMAT_A2B10G10R10_SINT_PACK32,
			VK_FORMAT_A2B10G10R10_UINT_PACK32
		};

		return typeTable[(UInt32)type];
	}

	FORCE_INLINE UInt32 AttributeSize(GFXAttributeType elementType)
	{
		switch (elementType)
		{
			case GFX_VERTEX_TYPE_FLOAT:				return 1 * sizeof(Float32);
			case GFX_VERTEX_TYPE_FLOAT2:			return 2 * sizeof(Float32);
			case GFX_VERTEX_TYPE_FLOAT3:			return 3 * sizeof(Float32);
			case GFX_VERTEX_TYPE_FLOAT4:			return 4 * sizeof(Float32);
			case GFX_VERTEX_TYPE_INT:				return 1 * sizeof(Int32);
			case GFX_VERTEX_TYPE_INT2:				return 2 * sizeof(Int32);
			case GFX_VERTEX_TYPE_INT3:				return 3 * sizeof(Int32);
			case GFX_VERTEX_TYPE_INT4:				return 4 * sizeof(Int32);
			case GFX_VERTEX_TYPE_UINT:				return 1 * sizeof(UInt32);
			case GFX_VERTEX_TYPE_UINT2:				return 2 * sizeof(UInt32);
			case GFX_VERTEX_TYPE_UINT3:				return 3 * sizeof(UInt32);
			case GFX_VERTEX_TYPE_UINT4:				return 4 * sizeof(UInt32);
			case GFX_VERTEX_TYPE_INT_2_10_10_10:	return 1 * sizeof(Int32);
			case GFX_VERTEX_TYPE_UINT_2_10_10_10:	return 1 * sizeof(UInt32);
			default: return 0;
		}
	}

	FORCE_INLINE VkPrimitiveTopology PrimitiveTopologyToVkPrimitiveTopology(GFXPrimitiveTopology topology)
	{
		static VkPrimitiveTopology topologyTable[] =
		{
			VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
			VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
		};

		return topologyTable[(UInt32)topology];
	}

	FORCE_INLINE VkPolygonMode PolygonModeToVkPolygonMode(GFXPolygonMode mode)
	{
		static VkPolygonMode modeTable[] =
		{
			VK_POLYGON_MODE_FILL,
			VK_POLYGON_MODE_LINE,
			VK_POLYGON_MODE_POINT
		};

		return modeTable[(UInt32)mode];
	}

	FORCE_INLINE VkCullModeFlags CullModeToVkCullMode(GFXCullMode mode)
	{
		static VkCullModeFlags modeTable[] =
		{
			VK_CULL_MODE_NONE,
			VK_CULL_MODE_FRONT_BIT,
			VK_CULL_MODE_BACK_BIT,
			VK_CULL_MODE_FRONT_AND_BACK
		};

		return modeTable[(UInt32)mode];
	}

	FORCE_INLINE VkFrontFace FaceWindToVkFrontFace(GFXFaceWind wind)
	{
		static VkFrontFace windTable[] =
		{
			VK_FRONT_FACE_COUNTER_CLOCKWISE,
			VK_FRONT_FACE_CLOCKWISE
		};

		return windTable[(UInt32)wind];
	}

	FORCE_INLINE UInt32 MultisampleToVkMultisample(GFXMultisample multisample)
	{
		//TODO: Add check for support

		static UInt32 multisampleTable[] =
		{
			1, 2, 4, 8
		};

		return multisampleTable[(UInt32)multisample];
	}

	FORCE_INLINE VkDescriptorType DescriptorTypeToVkDescriptorType(GFXDescriptorType type)
	{
		static VkDescriptorType typeTable[] =
		{
			VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
			VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
		};

		return typeTable[(UInt32)type];
	}

	FORCE_INLINE VkPipelineStageFlags ShaderStagesToVkShaderStages(GFXShaderStages stages)
	{
		VkPipelineStageFlags resultStages = 0;

		if ((stages & GFX_SHADER_STAGE_VERTEX_BIT) == GFX_SHADER_STAGE_VERTEX_BIT)
		{
			resultStages |= VK_SHADER_STAGE_VERTEX_BIT;
		}
		if ((stages & GFX_SHADER_STAGE_PIXEL_BIT) == GFX_SHADER_STAGE_PIXEL_BIT)
		{
			resultStages |= VK_SHADER_STAGE_FRAGMENT_BIT;
		}

		return resultStages;
	}
}