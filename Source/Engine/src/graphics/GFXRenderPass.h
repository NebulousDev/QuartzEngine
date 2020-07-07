#pragma once

#include "Common.h"
#include "util\Array.h"
#include "GFXImage.h"
#include "GFXResource.h"

namespace Quartz
{
	enum GFXLoadOp
	{
		GFX_LOAD_DONT_CARE,
		GFX_LOAD_LOAD,
		GFX_LOAD_CLEAR
	};

	enum GFXStoreOp
	{
		GFX_STORE_DONT_CARE,
		GFX_STORE_STORE
	};

	struct GFXColorAttachment
	{
		GFXFormat format;
		GFXLoadOp loadOp;
		GFXStoreOp storeOp;
	};

	struct GFXDepthStencilAttachment
	{
		GFXFormat format;
		GFXLoadOp depthLoadOp;
		GFXStoreOp depthStoreOp;
		GFXLoadOp stencilLoadOp;
		GFXStoreOp stencilStoreOp;
	};

	struct GFXRenderPassInfo
	{
		Array<GFXColorAttachment> colorTargets;
		Array<GFXDepthStencilAttachment> depthStencilTargets;
	};

	class QUARTZ_API GFXRenderPass : public GFXResource
	{

	};
}