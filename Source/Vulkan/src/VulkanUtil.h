#pragma once
#include "Common.h"
#include "Vulkan.h"

namespace Quartz
{
	/********************************************************************************************/


	INLINE VkFormat GetVkFormat(GFXFormat format)
	{
		switch (format)
		{
			case GFX_FORMAT_UNKNOWN:				return VK_FORMAT_UNDEFINED;
			
			case GFX_FORMAT_R32G32B32_SFLOAT:		return VK_FORMAT_R32G32B32_SFLOAT;
			case GFX_FORMAT_R32G32B32A32_SFLOAT:	return VK_FORMAT_R32G32B32A32_SFLOAT;

			case GFX_FORMAT_R8G8B8_UNORM:			return VK_FORMAT_R8G8B8_UNORM;
			case GFX_FORMAT_R8G8B8A8_UNORM:			return VK_FORMAT_R8G8B8A8_UNORM;
					 
			case GFX_FORMAT_B8G8R8_UNORM:			return VK_FORMAT_B8G8R8_UNORM;
			case GFX_FORMAT_B8G8R8A8_UNORM:			return VK_FORMAT_B8G8R8A8_UNORM;

			default: return VK_FORMAT_UNDEFINED;
		}
	}

	INLINE GFXFormat GetGFXImageFormat(VkFormat format)
	{
		switch (format)
		{
			case VK_FORMAT_UNDEFINED:				return GFX_FORMAT_UNKNOWN;

			case VK_FORMAT_R32G32B32_SFLOAT:		return GFX_FORMAT_R32G32B32_SFLOAT;
			case VK_FORMAT_R32G32B32A32_SFLOAT:		return GFX_FORMAT_R32G32B32A32_SFLOAT;

			case VK_FORMAT_R8G8B8_UNORM:			return GFX_FORMAT_R8G8B8_UNORM;
			case VK_FORMAT_R8G8B8A8_UNORM:			return GFX_FORMAT_R8G8B8A8_UNORM;

			case VK_FORMAT_B8G8R8_UNORM:			return GFX_FORMAT_B8G8R8_UNORM;
			case VK_FORMAT_B8G8R8A8_UNORM:			return GFX_FORMAT_B8G8R8A8_UNORM;

			default: return GFX_FORMAT_UNKNOWN;
		}
	}


	/********************************************************************************************/


	INLINE VkBufferUsageFlags GetVkUsageFlags(GFXBufferUsageFlags flags)
	{
		return (flags & GFX_BUFFER_USAGE_VERTEX_BIT) ? VK_BUFFER_USAGE_VERTEX_BUFFER_BIT : 0
			| (flags & GFX_BUFFER_USAGE_INDEX_BIT) ? VK_BUFFER_USAGE_INDEX_BUFFER_BIT : 0
			| (flags & GFX_BUFFER_USAGE_UNIFORM_BIT) ? VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT : 0
			| (flags & GFX_BUFFER_USAGE_STORAGE_BIT) ? VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT : 0
			| (flags & GFX_BUFFER_USAGE_TRANSFER_SRC_BIT) ? VK_BUFFER_USAGE_TRANSFER_SRC_BIT : 0
			| (flags & GFX_BUFFER_USAGE_TRANSFER_DEST_BIT) ? VK_BUFFER_USAGE_TRANSFER_DST_BIT : 0;
	}


	/********************************************************************************************/


	INLINE VkMemoryPropertyFlags GetVkMemoryPropertyFlags(GFXBufferMemoryFlags flags)
	{
		return (flags & GFX_BUFFER_MEMORY_VISIBLE_BIT) ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT : 0
			| (flags & GFX_BUFFER_MEMORY_COHERENT_BIT) ? VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0;
	}


	/********************************************************************************************/


	INLINE VkSharingMode GetVkSharingMode(GFXSharingMode mode)
	{
		switch (mode)
		{
			case GFX_SHARING_MODE_EXCLUSIVE:	return VK_SHARING_MODE_EXCLUSIVE;
			case GFX_SHARING_MODE_SHARED:		return VK_SHARING_MODE_EXCLUSIVE;
			default:							return VK_SHARING_MODE_EXCLUSIVE;
		}
	}


	/********************************************************************************************/


	INLINE VkImageViewType GetVkImageViewType(GFXImageType type)
	{
		switch (type)
		{
			case GFX_IMAGE_TYPE_1D:			return VK_IMAGE_VIEW_TYPE_1D;
			case GFX_IMAGE_TYPE_2D:			return VK_IMAGE_VIEW_TYPE_2D;
			case GFX_IMAGE_TYPE_3D:			return VK_IMAGE_VIEW_TYPE_3D;
			case GFX_IMAGE_TYPE_CUBE_MAP:	return VK_IMAGE_VIEW_TYPE_CUBE;
			default:						return VK_IMAGE_VIEW_TYPE_1D;
		}
	}


	/********************************************************************************************/


	INLINE VkShaderStageFlagBits GetVkShaderStageFlagBits(GFXShaderStage stage)
	{
		switch (stage)
		{
			case GFX_SHADER_STAGE_VERTEX:					return VK_SHADER_STAGE_VERTEX_BIT;
			case GFX_SHADER_STAGE_TESSELATION_CONTROL:		return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			case GFX_SHADER_STAGE_TESSELATION_EVALUATION:	return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			case GFX_SHADER_STAGE_GEOMETRY:					return VK_SHADER_STAGE_GEOMETRY_BIT;
			case GFX_SHADER_STAGE_FRAGMENT:					return VK_SHADER_STAGE_FRAGMENT_BIT;
			case GFX_SHADER_STAGE_COMPUTE:					return VK_SHADER_STAGE_COMPUTE_BIT;
			default:										return VK_SHADER_STAGE_ALL_GRAPHICS;
		}
	}


	/********************************************************************************************/


	INLINE VkShaderStageFlags GetVkShaderStageFlags(GFXShaderStageFlags stageFlags)
	{
		VkShaderStageFlags stageBits = 0;

		if (stageFlags & GFX_SHADER_STAGE_VERTEX)					stageBits |= VK_SHADER_STAGE_VERTEX_BIT;
		if (stageFlags & GFX_SHADER_STAGE_TESSELATION_CONTROL)		stageBits |= VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		if (stageFlags & GFX_SHADER_STAGE_TESSELATION_EVALUATION)	stageBits |= VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		if (stageFlags & GFX_SHADER_STAGE_GEOMETRY)					stageBits |= VK_SHADER_STAGE_GEOMETRY_BIT;
		if (stageFlags & GFX_SHADER_STAGE_FRAGMENT)					stageBits |= VK_SHADER_STAGE_FRAGMENT_BIT;
		if (stageFlags & GFX_SHADER_STAGE_COMPUTE)					stageBits |= VK_SHADER_STAGE_COMPUTE_BIT;

		return stageBits;
	}


	/********************************************************************************************/


	INLINE VkPrimitiveTopology GetVkPrimitiveTopology(GFXPrimitiveTopology topology)
	{
		switch (topology)
		{
			case GFX_PRIMITIVE_TOPOLOGY_POINTS:				return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			case GFX_PRIMITIVE_TOPOLOGY_LINES:				return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
			case GFX_PRIMITIVE_TOPOLOGY_LINE_STRIPS:		return VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
			case GFX_PRIMITIVE_TOPOLOGY_TRIANGLES:			return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			case GFX_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIPS:	return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
			default:										return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		}
	}


	/********************************************************************************************/


	INLINE VkPolygonMode GetVkPolygonMode(GFXFillMode fillMode)
	{
		switch (fillMode)
		{
			case GFX_POLYGON_FILL_MODE_FILL:	return VK_POLYGON_MODE_FILL;
			case GFX_POLYGON_FILL_MODE_LINE:	return VK_POLYGON_MODE_LINE;
			case GFX_POLYGON_FILL_MODE_POINT:	return VK_POLYGON_MODE_POINT;
			default:							return VK_POLYGON_MODE_FILL;
		}
	}


	/********************************************************************************************/


	INLINE VkCullModeFlagBits GetVkCullModeFlagBits(GFXCullFace cullFace)
	{
		switch (cullFace)
		{
			case GFX_CULL_FACE_NONE:			return VK_CULL_MODE_NONE;
			case GFX_CULL_FACE_FRONT:			return VK_CULL_MODE_FRONT_BIT;
			case GFX_CULL_FACE_BACK:			return VK_CULL_MODE_BACK_BIT;
			case GFX_CULL_FACE_FRONT_AND_BACK:	return VK_CULL_MODE_FRONT_AND_BACK;
			default:							return VK_CULL_MODE_NONE;
		}
	}


	/********************************************************************************************/


	INLINE VkFrontFace GetVkFrontFace(GFXFrontFace frontFace)
	{
		switch (frontFace)
		{
			case GFX_FRONT_FACE_CLOCKWISE:			return VK_FRONT_FACE_CLOCKWISE;
			case GFX_FRONT_FACE_COUNTER_CLOCKWISE:	return VK_FRONT_FACE_COUNTER_CLOCKWISE;
			default:								return VK_FRONT_FACE_CLOCKWISE;
		}
	}


	/********************************************************************************************/


	INLINE VkSampleCountFlagBits GetVkSampleCountFlagBits(UInt32 multisamples)
	{
		switch (multisamples)
		{
			case 0:		return VK_SAMPLE_COUNT_1_BIT;
			case 2:		return VK_SAMPLE_COUNT_2_BIT;
			case 4:		return VK_SAMPLE_COUNT_4_BIT;
			case 8:		return VK_SAMPLE_COUNT_8_BIT;
			case 16:	return VK_SAMPLE_COUNT_16_BIT;
			case 32:	return VK_SAMPLE_COUNT_32_BIT;
			default:	return VK_SAMPLE_COUNT_1_BIT;
		}
	}


	/********************************************************************************************/


	INLINE VkColorComponentFlags VkGetColorComponentFlags(GFXColorMask colorComponents)
	{
		VkColorComponentFlags colorComponentsRes = 0;

		if (colorComponents & GFX_COLOR_COMPONENT_R_BIT)
		{
			colorComponentsRes |= VK_COLOR_COMPONENT_R_BIT;
		}

		if (colorComponents & GFX_COLOR_COMPONENT_G_BIT)
		{
			colorComponentsRes |= VK_COLOR_COMPONENT_G_BIT;
		}

		if (colorComponents & GFX_COLOR_COMPONENT_B_BIT)
		{
			colorComponentsRes |= VK_COLOR_COMPONENT_B_BIT;
		}

		if (colorComponents & GFX_COLOR_COMPONENT_A_BIT)
		{
			colorComponentsRes |= VK_COLOR_COMPONENT_A_BIT;
		}
	
		return colorComponentsRes;
	}

	/********************************************************************************************/


	INLINE VkBlendFactor GetVkBlendFactor(GFXBlendFactor blendFactor)
	{
		switch (blendFactor)
		{
			case GFX_BLEND_FACTOR_ONE:							return VK_BLEND_FACTOR_ONE;
			case GFX_BLEND_FACTOR_ZERO:							return VK_BLEND_FACTOR_ZERO;
			case GFX_BLEND_FACTOR_SRC_COLOR:					return VK_BLEND_FACTOR_SRC_COLOR;
			case GFX_BLEND_FACTOR_ONE_MINUS_SRC_COLOR:			return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
			case GFX_BLEND_FACTOR_DST_COLOR:					return VK_BLEND_FACTOR_DST_COLOR;
			case GFX_BLEND_FACTOR_ONE_MINUS_DST_COLOR:			return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
			case GFX_BLEND_FACTOR_CONSTANT_COLOR:				return VK_BLEND_FACTOR_CONSTANT_COLOR;
			case GFX_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR:		return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR;
			case GFX_BLEND_FACTOR_SRC_ALPHA:					return VK_BLEND_FACTOR_SRC_ALPHA;
			case GFX_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:			return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			case GFX_BLEND_FACTOR_DST_ALPHA:					return VK_BLEND_FACTOR_DST_ALPHA;
			case GFX_BLEND_FACTOR_ONE_MINUS_DST_ALPHA:			return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
			case GFX_BLEND_FACTOR_CONSTANT_ALPHA:				return VK_BLEND_FACTOR_CONSTANT_ALPHA;
			case GFX_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA:		return VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA;
			default:											return VK_BLEND_FACTOR_ONE;
		}
	}


	/********************************************************************************************/


	INLINE VkBlendOp GetVkBlendOp(GFXBlendOp blendOp)
	{
		switch (blendOp)
		{
			case GFX_BLEND_OP_ADD:				return VK_BLEND_OP_ADD;
			case GFX_BLEND_OP_SUBTRACT:			return VK_BLEND_OP_SUBTRACT;
			case GFX_BLEND_OP_REVERSE_SUBTRACT:	return VK_BLEND_OP_REVERSE_SUBTRACT;
			case GFX_BLEND_OP_MIN:				return VK_BLEND_OP_MIN;
			case GFX_BLEND_OP_MAX:				return VK_BLEND_OP_MAX;
			default:							return VK_BLEND_OP_ADD;
		}
	}


	/********************************************************************************************/


	INLINE VkLogicOp GetVkLogicalOp(GFXLogicalOp logicalOp)
	{
		switch (logicalOp)
		{
			case GFX_LOGICAL_OP_CLEAR:			return VK_LOGIC_OP_CLEAR;
			case GFX_LOGICAL_OP_AND:			return VK_LOGIC_OP_AND;
			case GFX_LOGICAL_OP_AND_REVERSE: 	return VK_LOGIC_OP_AND_REVERSE;
			case GFX_LOGICAL_OP_COPY: 			return VK_LOGIC_OP_COPY;
			case GFX_LOGICAL_OP_AND_INVERTED: 	return VK_LOGIC_OP_AND_INVERTED;
			case GFX_LOGICAL_OP_NO_OP: 			return VK_LOGIC_OP_NO_OP;
			case GFX_LOGICAL_OP_XOR: 			return VK_LOGIC_OP_XOR;
			case GFX_LOGICAL_OP_OR: 			return VK_LOGIC_OP_OR;
			case GFX_LOGICAL_OP_NOR: 			return VK_LOGIC_OP_NOR;
			case GFX_LOGICAL_OP_EQUIVALENT: 	return VK_LOGIC_OP_EQUIVALENT;
			case GFX_LOGICAL_OP_INVERT: 		return VK_LOGIC_OP_INVERT;
			case GFX_LOGICAL_OP_OR_REVERSE: 	return VK_LOGIC_OP_OR_REVERSE;
			case GFX_LOGICAL_OP_COPY_INVERTED: 	return VK_LOGIC_OP_COPY_INVERTED;
			case GFX_LOGICAL_OP_OR_INVERTED: 	return VK_LOGIC_OP_OR_INVERTED;
			case GFX_LOGICAL_OP_NAND: 			return VK_LOGIC_OP_NAND;
			case GFX_LOGICAL_OP_SET: 			return VK_LOGIC_OP_SET;
			default:							return VK_LOGIC_OP_CLEAR;
		}
	}


	/********************************************************************************************/


	INLINE VkAttachmentLoadOp GetVkAttachmentLoadOp(GFXLoadOp loadOp)
	{
		switch (loadOp)
		{
			case GFX_LOAD_OP_LOAD:		return VK_ATTACHMENT_LOAD_OP_LOAD;
			case GFX_LOAD_OP_CLEAR:		return VK_ATTACHMENT_LOAD_OP_CLEAR;
			case GFX_LOAD_OP_DONT_CARE:	return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			default:					return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		}
	}


	/********************************************************************************************/


	INLINE VkAttachmentStoreOp GetVkAttachmentStoreOp(GFXStoreOp storeOp)
	{
		switch (storeOp)
		{
			case GFX_STORE_OP_STORE:		return VK_ATTACHMENT_STORE_OP_STORE;
			case GFX_STORE_OP_DONT_CARE:	return VK_ATTACHMENT_STORE_OP_DONT_CARE;
			default:						return VK_ATTACHMENT_STORE_OP_DONT_CARE;
		}
	}


	/********************************************************************************************/


	INLINE VkImageLayout GetVkImageLayout(GFXImageLayout imageLayout)
	{
		switch (imageLayout)
		{
			case GFX_IMAGE_LAYOUT_UNDEFINED:							return VK_IMAGE_LAYOUT_UNDEFINED;
			case GFX_IMAGE_LAYOUT_GENERAL: 								return VK_IMAGE_LAYOUT_GENERAL;
			case GFX_IMAGE_LAYOUT_COLOR_ATTACHMENT: 					return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			case GFX_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT:				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			case GFX_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY: 				return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
			case GFX_IMAGE_LAYOUT_SHADER_READ_ONLY: 					return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			case GFX_IMAGE_LAYOUT_TRANSFER_SRC: 						return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			case GFX_IMAGE_LAYOUT_TRANSFER_DEST: 						return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			case GFX_IMAGE_LAYOUT_PREINITIALIZED: 						return VK_IMAGE_LAYOUT_PREINITIALIZED;
			case GFX_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT: 	return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
			case GFX_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY: 	return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
			case GFX_IMAGE_LAYOUT_PRESENT_SRC: 							return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			case GFX_IMAGE_LAYOUT_SHARED_PRESENT: 						return VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR;
			default:													return VK_IMAGE_LAYOUT_UNDEFINED;
		}
	}


	/********************************************************************************************/


	INLINE VkDescriptorType GetVkDescriptorType(GFXDescriptorType type)
	{
		switch (type)
		{
			case GFX_DESCRIPTOR_TYPE_UNIFORM:	return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			default:							return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		}
	}


	/********************************************************************************************/


	INLINE VkPipelineBindPoint GetVkPipelineBindPoint(GFXPipelineBindPoint bindPoint)
	{
		switch (bindPoint)
		{
			case GFX_PIPELINE_BIND_POINT_GRAPHICS:	return VK_PIPELINE_BIND_POINT_GRAPHICS;
			case GFX_PIPELINE_BIND_POINT_COMPUTE:	return VK_PIPELINE_BIND_POINT_COMPUTE;
			default:								return VK_PIPELINE_BIND_POINT_GRAPHICS;
		}
	}


	/********************************************************************************************/


	INLINE VkPipelineStageFlags GetVkPipelineStageFlags(GFXPipelineStageMask stageMask)
	{
		switch (stageMask)
		{
			case GFX_PIPELINE_STAGE_NONE:								return 0;
			case GFX_PIPELINE_STAGE_TOP_OF_PIPE_BIT:					return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			case GFX_PIPELINE_STAGE_DRAW_INDIRECT_BIT:					return VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT;
			case GFX_PIPELINE_STAGE_VERTEX_INPUT_BIT:					return VK_PIPELINE_STAGE_VERTEX_INPUT_BIT;
			case GFX_PIPELINE_STAGE_VERTEX_SHADER_BIT:					return VK_PIPELINE_STAGE_VERTEX_SHADER_BIT;
			case GFX_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT:	return VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT;
			case GFX_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT:	return VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT;
			case GFX_PIPELINE_STAGE_GEOMETRY_SHADER_BIT:				return VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
			case GFX_PIPELINE_STAGE_FRAGMENT_SHADER_BIT:				return VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT;
			case GFX_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT:		return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			case GFX_PIPELINE_STAGE_COMPUTE_SHADER_BIT:					return VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT;
			case GFX_PIPELINE_STAGE_TRANSFER_BIT:						return VK_PIPELINE_STAGE_TRANSFER_BIT;
			case GFX_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT:					return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
			case GFX_PIPELINE_STAGE_HOST_BIT:							return VK_PIPELINE_STAGE_HOST_BIT;
			default:													return 0;
		}
	}


	/********************************************************************************************/


	INLINE VkAccessFlags GetVkAccessFlags(GFXAccessMask accessMask)
	{
		switch (accessMask)
		{
			case GFX_ACCESS_NONE:									return 0;
			case GFX_ACCESS_INDIRECT_COMMAND_READ_BIT:				return VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
			case GFX_ACCESS_INDEX_READ_BIT:							return VK_ACCESS_INDEX_READ_BIT;
			case GFX_ACCESS_VERTEX_ATTRIBUTE_READ_BIT:				return VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
			case GFX_ACCESS_UNIFORM_READ_BIT:						return VK_ACCESS_UNIFORM_READ_BIT;
			case GFX_ACCESS_INPUT_ATTACHMENT_READ_BIT:				return VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
			case GFX_ACCESS_SHADER_READ_BIT:						return VK_ACCESS_SHADER_READ_BIT;
			case GFX_ACCESS_SHADER_WRITE_BIT:						return VK_ACCESS_SHADER_WRITE_BIT;
			case GFX_ACCESS_COLOR_ATTACHMENT_READ_BIT:				return VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
			case GFX_ACCESS_COLOR_ATTACHMENT_WRITE_BIT:				return VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			case GFX_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT:		return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
			case GFX_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT:		return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
			case GFX_ACCESS_TRANSFER_READ_BIT:						return VK_ACCESS_TRANSFER_READ_BIT;
			case GFX_ACCESS_TRANSFER_WRITE_BIT:						return VK_ACCESS_TRANSFER_WRITE_BIT;
			case GFX_ACCESS_HOST_READ_BIT:							return VK_ACCESS_HOST_READ_BIT;
			case GFX_ACCESS_HOST_WRITE_BIT:							return VK_ACCESS_HOST_WRITE_BIT;
			case GFX_ACCESS_MEMORY_READ_BIT:						return VK_ACCESS_MEMORY_READ_BIT;
			case GFX_ACCESS_MEMORY_WRITE_BIT:						return VK_ACCESS_MEMORY_WRITE_BIT;
			default:												return 0;
		}
	}


	/********************************************************************************************/


	INLINE VkVertexInputRate GetVkVertexInputRate(GFXVertexInputRate vertexInputRate)
	{
		switch (vertexInputRate)
		{
		case Quartz::GFX_VERTEX_INPUT_RATE_VERTEX:		return VK_VERTEX_INPUT_RATE_VERTEX;
		case Quartz::GFX_VERTEX_INPUT_RATE_INSTANCE:	return VK_VERTEX_INPUT_RATE_INSTANCE;
		default:										return VK_VERTEX_INPUT_RATE_VERTEX;
		}
	}


	/********************************************************************************************/
}