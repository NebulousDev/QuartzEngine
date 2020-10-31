#pragma once

#include "../../entity/System.h"

#include "../GFXContext.h"
#include "util\Array.h"

#include "..\..\object\Model.h"
#include "..\..\object\UniformData.h"
#include "math\Math.h"

namespace Quartz
{
	class QUARTZ_API RenderSystem : public SingletonSystem
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

		GFXContext* mpGfx;
		Window* mpWindow;

		Model mModel;

		GFXRenderPass* mpRenderPass;

		GFXImageView* mpDiffuse;

		Matrix4 mModelMatrix, mViewMatrix, mProjectionMatrix;

	private:
		void CreateSurface();
		void CreateTextures();
		void CreateGeometry();
		void CreatePipeline();

	public:
		void OnInit(EntityWorld& world) override;
		void OnUpdate(EntityWorld& world, Float32 deltaTime) override;
		void OnTick(EntityWorld& world, Float32 deltaTime) override;
		void OnDestroy(EntityWorld& world) override;
	};
}