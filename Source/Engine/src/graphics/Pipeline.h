#pragma once

#include "util/Array.h"

#include "Shader.h"
#include "RenderPass.h"

namespace Quartz
{
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
		UInt32			binding;
		UInt32			location;
		AttributeType	type;
	};

	struct ColorBlend
	{
		//TODO: Implement
	};

	struct BlendAttachment
	{
		ColorBlend blend;
	};

	struct ViewportArea
	{
		Float32 x;
		Float32 y;
		Float32 width;
		Float32 height;
		Float32 minDepth;
		Float32 maxDepth;
	};

	struct ScissorArea
	{
		Float32 x;
		Float32 y;
		Float32 width;
		Float32 height;
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
		CULL_MODE_BACK,
		CULL_MODE_FRONT_AND_BACK
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

	enum PipelineType
	{
		PIPELINE_TYPE_GRAPHICS,
		PIPELINE_TYPE_COMPUTE
	};

	struct GraphicsPipelineInfo
	{
		Array<Shader*>			shaders;
		Array<BufferAttachent>	bufferAttachments;
		Array<VertexAttribute>	vertexAttributes;
		//Array<GFXDescriptorAttachment> descriptorAttachents;
		Array<BlendAttachment>	blendAttachments;
		ViewportArea			viewport;
		ScissorArea				scissor;
		PrimitiveTopology		topology;
		PolygonMode				polygonMode;
		CullMode				cullMode;
		FaceWind				faceWind;
		Float32					lineWidth;
		Multisample				multisample;
		Bool8					depthTesting;
		CompareOperation		depthOperation;
		Renderpass*				pRenderpass;
	};

	class QUARTZ_API Pipeline
	{
	private:
		PipelineType mType;

	protected:
		Pipeline(PipelineType type);

	public:
		FORCE_INLINE PipelineType GetPipelineType() const { return mType; }
	};

	class QUARTZ_API GraphicsPipeline : public Pipeline
	{
	protected:
		GraphicsPipelineInfo mPipelineInfo;

	protected:
		GraphicsPipeline(const GraphicsPipelineInfo& info);

	public:

		// void SetDynamicViewport(Context);
	};
}