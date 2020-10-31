#include "RenderSystem.h"

#include "util\Array.h"
#include "..\..\log\Log.h"
#include "../../Engine.h"
#include "../../object/OBJLoader.h"

#include <iostream>
#include <string>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//I dont like Camera stuff in here
#include "CameraSystem.h"

#define MODEL_PATH "models/testscene.obj"
#define DIFFUSE_PATH "textures/default.png"

namespace Quartz
{
	// Get this out of here asap
	static Array<Byte> ReadFile(const String& filename)
	{
		std::ifstream file(filename.Str(), std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			Log.Error("Cannot open file %s", filename.Str());
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		Array<Byte> buffer(fileSize);

		file.seekg(0);
		file.read((char*)buffer.Data(), fileSize);

		file.close();

		return buffer;
	}

	// Should be moved out of RenderSystem
	// A Surface should be passed into the render function
	void RenderSystem::CreateSurface()
	{
		mpSurface = mpGfx->CreateSurface(*mpWindow, mpWindow->Width(), mpWindow->Height(), true, false);
	}

	// Should be moved out of RenderSystem
	// Texture creation should be handled by it's own system
	void RenderSystem::CreateTextures()
	{
		// This is too much per texture

		Int32 texWidth, texHeight, texChannels;
		stbi_uc* pixels = stbi_load(DIFFUSE_PATH, &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
		GFXImage* pDiffuse = mpGfx->CreateImage(IMAGE_USAGE_SAMPLED_TEXTURE, texWidth, texHeight, FORMAT_RGBA8_UNORM, 1, 1);
		GFXTextureBuffer* pDiffuseTextureBuffer = mpGfx->CreateTextureBuffer(pDiffuse->GetSizeBytes(), true);
		void* pTextureData = mpGfx->MapTextureBuffer(pDiffuseTextureBuffer);
		memcpy(pTextureData, pixels, pDiffuse->GetSizeBytes());
		mpGfx->UnmapTextureBuffer(pDiffuseTextureBuffer);
		mpGfx->CopyTextureBufferToImage(pDiffuseTextureBuffer, pDiffuse);
		mpDiffuse = mpGfx->CreateImageView(IMAGE_USAGE_SAMPLED_TEXTURE, pDiffuse, 0, 1, 0, 1);
		// Destroy TextureBuffer
	}

	// Should be moved out of RenderSystem
	// Model creation should be handled by it's own system
	void RenderSystem::CreateGeometry()
	{
		// Hard-coded load of a single model:

		Array<Byte> modelFileData = ReadFile(MODEL_PATH);
		String modelDataString = String((char*)modelFileData.Data(), modelFileData.Size());

		mModel = LoadOBJ(modelDataString);

		mpVertexBuffer = mpGfx->CreateVertexBuffer(mModel.vertexData.buffer.Size(), true);
		void* pVertexData = mpGfx->MapVertexBuffer(mpVertexBuffer);
		memcpy(pVertexData, mModel.vertexData.buffer.Data(), mpVertexBuffer->GetSizeBytes());
		mpGfx->UnmapVertexBuffer(mpVertexBuffer);

		mpIndexBuffer = mpGfx->CreateIndexBuffer(sizeof(UInt32), mModel.indexData.buffer.Size() / sizeof(UInt32), true);
		void* pIndexData = mpGfx->MapIndexBuffer(mpIndexBuffer);
		memcpy(pIndexData, mModel.indexData.buffer.Data(), mpIndexBuffer->GetSizeBytes());
		mpGfx->UnmapIndexBuffer(mpIndexBuffer);

		// Model Uniforms:

		mModelMatrix.SetTranslation({ 0.0f, 0.0f, 0.0f });
		mViewMatrix.SetTranslation({ 0.0f, 0.0f, 5.0f });
		mProjectionMatrix.SetIdentity();
		mProjectionMatrix.SetPerspective(ToRadians(90.0f), (Float32)mpWindow->Width() / (Float32)mpWindow->Height(), 0.001f, 1000.0f);

		mModelUniform.AddMatrix4("model", mModelMatrix);
		mModelUniform.AddMatrix4("view", mViewMatrix);
		mModelUniform.AddMatrix4("projection", mProjectionMatrix);
	}

	void RenderSystem::CreatePipeline()
	{
		GraphicsPipelineState pipelineState;

		pipelineState.pVertexShader = mpGfx->CreateVertexShader(ReadFile("shaders/vert.spv"));
		pipelineState.pPixelShader = mpGfx->CreatePixelShader(ReadFile("shaders/frag.spv"));

		Bounds2f renderBounds(0, 0, mpWindow->Width(), mpWindow->Height());

		pipelineState.viewport.bounds = renderBounds;
		pipelineState.viewport.minDepth = 0.0f;
		pipelineState.viewport.maxDepth = 1.0f;

		pipelineState.scissor.bounds = renderBounds;

		pipelineState.topology = PRIMITIVE_TOPOLOGY_TRIANGLES;
		pipelineState.polygonMode = POLYGON_MODE_FILL;
		pipelineState.cullMode = CULL_MODE_NONE;
		pipelineState.faceWind = FACE_WIND_COUNTER_CLOCKWISE;
		pipelineState.lineWidth = 1.0f;

		pipelineState.multisample = MULTISAMPLE_DISABLED;

		pipelineState.depthTesting = true;
		pipelineState.depthOperation = COMPARE_OP_LESS_OR_EQUAL;
	
		BufferAttachent vertexBufferAttachment;
		vertexBufferAttachment.binding = 0;
		vertexBufferAttachment.stride = 8 * sizeof(Float32);
		pipelineState.bufferAttachemnts.PushBack(vertexBufferAttachment);

		VertexAttribute positionAttrib;
		positionAttrib.binding = 0;
		positionAttrib.location = 0;
		positionAttrib.type = ATTRIBUTE_TYPE_FLOAT3;
		pipelineState.vertexAttributes.PushBack(positionAttrib);

		VertexAttribute normalAttrib;
		normalAttrib.binding = 0;
		normalAttrib.location = 1;
		normalAttrib.type = ATTRIBUTE_TYPE_FLOAT3;
		pipelineState.vertexAttributes.PushBack(normalAttrib);

		VertexAttribute texCoordAttrib;
		texCoordAttrib.binding = 0;
		texCoordAttrib.location = 2;
		texCoordAttrib.type = ATTRIBUTE_TYPE_FLOAT2;
		pipelineState.vertexAttributes.PushBack(texCoordAttrib);

		DescriptorAttachment uniformAttachment;
		uniformAttachment.binding = 0;
		uniformAttachment.arraySize = 1;
		uniformAttachment.type = DESCRIPTOR_TYPE_UNIFORM;
		uniformAttachment.stages = SHADER_STAGE_VERTEX;

		pipelineState.descriptorAttachents.PushBack(uniformAttachment);

		BlendAttachment colorOutputBlendAttachment;
		colorOutputBlendAttachment.blend = ColorBlend();
		pipelineState.blendAttachments.PushBack(colorOutputBlendAttachment);

		GFXRenderPassInfo renderPassInfo = {};
		renderPassInfo.colorTargets.PushBack({ FORMAT_BGRA8_UNORM, GFX_LOAD_DONT_CARE, GFX_STORE_STORE }); // Swapchain image
		renderPassInfo.depthStencilTargets.PushBack({ FORMAT_DEPTH24_UNORM_STENCIL8_UINT, GFX_LOAD_CLEAR, GFX_STORE_STORE }); // depthStencil image
		mpRenderPass = mpGfx->CreateRenderPass(renderPassInfo);

		mpGraphicsPipeline = mpGfx->CreateGraphicsPipeline2(pipelineState, *mpRenderPass);
	}

	void RenderSystem::OnInit(EntityWorld& world)
	{
		mpGfx = &Engine::GetInstanceHandle().GetGFXContext();
		mpWindow = &Engine::GetInstanceHandle().GetWindow();

		CreateSurface();
		CreateTextures();
		CreateGeometry();
		CreatePipeline();

		Array<GFXImage*> depthImages;

		for (UInt32 i = 0; i < mpSurface->GetBackBufferCount(); i++)
		{
			GFXUniformBuffer* pUniformBuffer = mpGfx->CreateUniformBuffer(mModelUniform.GetBuffer().Size() * sizeof(Float32), true);

			void* pUniformData = mpGfx->MapUniformBuffer(pUniformBuffer);
			memcpy(pUniformData, mModelUniform.GetBuffer().Data(), pUniformBuffer->GetSizeBytes());
			mpGfx->MapUniformBuffer(pUniformBuffer);

			mUniformBuffers.PushBack(pUniformBuffer);

			GFXImage* pDepthImage = mpGfx->CreateImage(IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT, mpSurface->GetWidth(), mpSurface->GetHeight(), FORMAT_DEPTH24_UNORM_STENCIL8_UINT, 1, 1);
			depthImages.PushBack(pDepthImage);

			Array<GFXImageView*> framebufferImages;
			framebufferImages.PushBack(&mpSurface->GetImageView(i));
			framebufferImages.PushBack(mpGfx->CreateImageView(IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT, pDepthImage, 0, 1, 0, 1));

			GFXFramebuffer* pPresentFramebuffer = mpGfx->CreateFramebuffer(mpGraphicsPipeline, mpSurface->GetWidth(), mpSurface->GetHeight(), framebufferImages);
			mPresentFramebuffers.PushBack(pPresentFramebuffer);

			GFXCommandBuffer* pGraphicsCommandBuffer = mpGfx->CreateGraphicsCommandBuffer();
			mGraphicsCommandBuffers.PushBack(pGraphicsCommandBuffer);

			pGraphicsCommandBuffer->Begin();
			mpGfx->BeginRenderPass(*pGraphicsCommandBuffer, *mpRenderPass, *pPresentFramebuffer);

			mpGfx->BindGraphicsPipeline(*pGraphicsCommandBuffer, *mpGraphicsPipeline);
			mpGfx->BindVertexBuffer(*pGraphicsCommandBuffer, mpVertexBuffer);
			mpGfx->BindIndexBuffer(*pGraphicsCommandBuffer, mpIndexBuffer);
			mpGfx->BindUniformBuffer(*mpGraphicsPipeline, *pGraphicsCommandBuffer, pUniformBuffer);

			mpGfx->DrawIndexed(*pGraphicsCommandBuffer, mpIndexBuffer->GetCount(), 0);

			mpGfx->EndRenderPass(*pGraphicsCommandBuffer);
			pGraphicsCommandBuffer->End();
		}

		world.RegisterSystem<CameraSystem>();
	}

	void RenderSystem::OnUpdate(EntityWorld& world, Float32 deltaTime)
	{
		// Note: command buffers will need to be rebuilt for each camera
		// or more likely n-buffer them
		for (Entity entity : world.CreateView<Camera, Transform>())
		{
			Camera& camera = world.GetComponent<Camera>(entity);
			Transform& cameraTransfrom = world.GetComponent<Transform>(entity);

			mFrameIndex = mpSurface->AcquireNextImageIndex();

			/* Model */
			//mModelMatrix *= Matrix4().SetRotation(Quaternion().SetAxisAngle({ 0.0f, 1.0f, 0.0f }, 1.0f * Engine::GetInstanceHandle().GetDeltaTime()));
			//mModelUniform.SetMatrix4("model", mModelMatrix);

			/* View */
			Matrix4 view = Matrix4().SetTranslation(cameraTransfrom.position) * Matrix4().SetRotation(cameraTransfrom.orientation);
			mModelUniform.SetMatrix4("view", view);

			/* Projection */
			mModelUniform.SetMatrix4("projection", camera.projection);

			void* pUniformData = mpGfx->MapUniformBuffer(mUniformBuffers[mFrameIndex]);
			memcpy(pUniformData, mModelUniform.GetBuffer().Data(), mUniformBuffers[mFrameIndex]->GetSizeBytes());
			mpGfx->MapUniformBuffer(mUniformBuffers[mFrameIndex]);

			mpGfx->Submit(*mGraphicsCommandBuffers[mFrameIndex], *mpSurface);
			mpGfx->Present(mpSurface);
		}
	}

	void RenderSystem::OnTick(EntityWorld& world, Float32 deltaTime)
	{

	}

	void RenderSystem::OnDestroy(EntityWorld& world)
	{

	}
}

