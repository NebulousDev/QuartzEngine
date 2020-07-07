#pragma once

#include "../system/System.h"

#include "GFXContext.h"
#include "util\Array.h"

namespace Quartz
{
	class QUARTZ_API RenderSystem : public System
	{
	private:
		GFXSurface*				mpSurface;
		GFXGraphicsPipeline*	mpGraphicsPipeline;
		GFXVertexBuffer*		mpVertexBuffer;
		GFXIndexBuffer*			mpIndexBuffer;
		
		Array<GFXUniformBuffer*>	mUniformBuffers;
		Array<GFXFramebuffer*>		mPresentFramebuffers;
		Array<GFXCommandBuffer*>	mGraphicsCommandBuffers;

		UInt32 mFrameIndex;

	public:
		void Initialize() override;
		void Update() override;
		void Tick() override;
		void Destroy() override;
	};
}