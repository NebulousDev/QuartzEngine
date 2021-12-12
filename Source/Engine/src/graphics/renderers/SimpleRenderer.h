#pragma once

#include "../Renderer.h"
#include "../Framebuffer.h"
#include "../CommandBuffer.h"

#include "../../Engine.h"

#include "math/Math.h"

namespace Quartz
{
	struct Light
	{
		Vector3 position;
		UInt32	padding1;
		Vector3 radiance;
		UInt32	padding2;
	};

	struct PerFrameObjectData
	{
		Matrix4 model;
	};

	struct PerFrameUBOData
	{
		Matrix4 view;
		Matrix4 proj;
		Vector3 cameraPos;
		UInt32	padding1;
		Light	lights[16];
		UInt32	lightCount;
		UInt32	padding[3];
	};

	class QUARTZ_API SimpleRenderer : public Renderer
	{
	private:
		Renderpass*			mpRenderpass;
		Framebuffer*		mpFramebuffer;
		GraphicsPipeline*	mpGraphicsPipeline;
		CommandBuffer*		mpCommandBuffer;

		GFXShader* mpVertexShader;
		GFXShader* mpFragmentShader;

		PerFrameUBOData mPerFrameData;
		Array<PerFrameObjectData> mPerSetObjects;

		Uniform* mpPerFrame;
		Uniform* mpPerObject;

		UniformTextureSampler* mpDiffuse;
		UniformTextureSampler* mpNormal;
		UniformTextureSampler* mpRoughness;
		UniformTextureSampler* mpMetallic;
		UniformTextureSampler* mpAmbient;

	public:
		SimpleRenderer();

		void Setup(Context* pViewport) override;
		void Render(Context* pViewport, Scene* pScene) override;
		void Rebuild(Context* pViewport) override;
		void Destroy() override;
	};
}