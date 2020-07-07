#pragma once

#include "Common.h"
#include "GFXResource.h"

#include "GFXShaders.h"

namespace Quartz
{
	enum GFXPrimitiveTopology
	{
		GFX_PRIMITIVE_TOPOLOGY_POINTS,
		GFX_PRIMITIVE_TOPOLOGY_LINES,
		GFX_PRIMITIVE_TOPOLOGY_TRIANGLES
	};

	struct QUARTZ_API GFXGraphicsPipelineShaderState
	{
		GFXVertexShader*	pVertexShader;
		GFXPixelShader*		pPixelShader;
		GFXHullShader*		pHullShader;
		GFXDomainShader*	pDomainShader;
		GFXGeometryShader*	pGeometryShader;
	};

	class QUARTZ_API GFXGraphicsPipelineState
	{
	private:
		GFXGraphicsPipelineShaderState mShaderState;
		GFXPrimitiveTopology mTopology;

	public:
		GFXGraphicsPipelineState() = default;
		GFXGraphicsPipelineState(
			GFXGraphicsPipelineShaderState& shaderState,
			GFXPrimitiveTopology topology)
			: mShaderState(shaderState), mTopology(topology) {}
	};

	class QUARTZ_API GFXGraphicsPipeline : public GFXResource
	{
	private:
		int temp;

	public:
		GFXGraphicsPipeline() = default;
	};

	class QUARTZ_API GFXComputePipeline : public GFXResource
	{
	private:
		int temp;

	public:
		GFXComputePipeline() = default;
	};
}