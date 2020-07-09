#pragma once

#include "../system/System.h"

#include "GFXContext.h"
#include "util\Array.h"

#include "..\object\Model.h"
#include "..\object\UniformData.h"
#include "math\Math.h"

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

		UniformBlockData mModelUniform;

		UInt32 mFrameIndex;

		Matrix4 mModel, mView, mProjection;

	public:
		void Initialize() override;
		void Update() override;
		void Tick() override;
		void Destroy() override;
	};
}