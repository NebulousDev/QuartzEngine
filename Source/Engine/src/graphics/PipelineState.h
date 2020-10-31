#pragma once

#include "GFXPipelines.h"
#include "math/Bounds.h"
#include "util/Array.h"

namespace Quartz
{
	struct Viewport
	{
		Bounds2f bounds;
		Float32 minDepth;
		Float32 maxDepth;
	};

	struct Scissor
	{
		Bounds2f bounds;
	};

	struct ColorBlend
	{
		//TODO: Implement
	};

	struct BlendAttachment
	{
		ColorBlend blend;
	};

	enum PrimitiveTopology
	{
		PRIMITIVE_TOPOLOGY_POINTS,
		PRIMITIVE_TOPOLOGY_LINES,
		PRIMITIVE_TOPOLOGY_TRIANGLES
	};

	enum PolygonMode
	{
		POLYGON_MODE_FILL,
		POLYGON_MODE_LINE,
		POLYGON_MODE_POINT,
	};

	enum CullMode
	{
		CULL_MODE_NONE,
		CULL_MODE_FRONT,
		CULL_MODE_BACK
	};

	enum FaceWind
	{
		FACE_WIND_COUNTER_CLOCKWISE,
		FACE_WIND_CLOCKWISE
	};

	enum Multisample
	{
		MULTISAMPLE_DISABLED,
		MULTISAMPLE_2X,
		MULTISAMPLE_4X,
		MULTISAMPLE_8X
	};

	enum CompareOperation
	{
		COMPARE_OP_NONE,
		COMPARE_OP_LESS,
		COMPARE_OP_EQUAL,
		COMPARE_OP_LESS_OR_EQUAL,
		COMPARE_OP_GREATER,
		COMPARE_OP_NOT_EQUAL,
		COMPARE_OP_GREATER_OR_EQUAL,
		COMPARE_OP_ALWAYS,
	};

	struct BufferAttachent
	{
		UInt32 binding;
		UInt32 stride;
	};

	enum AttributeType
	{
		ATTRIBUTE_TYPE_FLOAT,
		ATTRIBUTE_TYPE_FLOAT2,
		ATTRIBUTE_TYPE_FLOAT3,
		ATTRIBUTE_TYPE_FLOAT4,
		ATTRIBUTE_TYPE_INT,
		ATTRIBUTE_TYPE_INT2,
		ATTRIBUTE_TYPE_INT3,
		ATTRIBUTE_TYPE_INT4,
		ATTRIBUTE_TYPE_UINT,
		ATTRIBUTE_TYPE_UINT2,
		ATTRIBUTE_TYPE_UINT3,
		ATTRIBUTE_TYPE_UINT4,
		ATTRIBUTE_TYPE_INT_2_10_10_10,
		ATTRIBUTE_TYPE_UINT_2_10_10_10
	};

	struct VertexAttribute
	{
		UInt32 binding;
		UInt32 location;
		AttributeType type;
	};

	enum DescriptorType
	{
		DESCRIPTOR_TYPE_UNIFORM,
		DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER
	};

	enum ShaderStageFlagBits
	{
		SHADER_STAGE_VERTEX = 0x1,
		SHADER_STAGE_FRAGMENT = 0x2
	};

	typedef Flags32 ShaderStages;

	struct DescriptorAttachment
	{
		UInt32 binding;
		DescriptorType type;
		UInt32 arraySize;
		ShaderStages stages;
	};

	enum LoadOperation
	{
		LOAD_OP_DONT_CARE,
		LOAD_OP_LOAD,
		LOAD_OP_CLEAR
	};

	enum StoreOperation
	{
		STORE_OP_DONT_CARE,
		STORE_OP_STORE
	};

	struct ColorAttachment
	{
		GFXFormat format;
		LoadOperation loadOp;
		StoreOperation storeOp;
	};

	struct DepthStencilAttachment
	{
		GFXFormat format;
		LoadOperation depthLoadOp;
		StoreOperation depthStoreOp;
		LoadOperation stencilLoadOp;
		StoreOperation stencilStoreOp;
	};

	struct GraphicsPipelineState
	{
		GFXVertexShader* pVertexShader;
		GFXPixelShader* pPixelShader;
		GFXHullShader* pHullShader;
		GFXDomainShader* pDomainShader;
		GFXGeometryShader* pGeometryShader;

		Array<BufferAttachent> bufferAttachemnts;
		Array<VertexAttribute> vertexAttributes;
		Array<DescriptorAttachment> descriptorAttachents;
		Array<BlendAttachment> blendAttachments;

		Array<ColorAttachment> colorTargets;
		Array<DepthStencilAttachment> depthStencilTargets;

		Viewport viewport;
		Scissor scissor;

		PrimitiveTopology topology;

		PolygonMode	polygonMode;
		CullMode cullMode;
		FaceWind faceWind;
		Float32 lineWidth;

		Multisample multisample;

		Bool8 depthTesting;
		CompareOperation depthOperation;
	};

	class QUARTZ_API GraphicsPipelineBuilder
	{
		
		
	};
}