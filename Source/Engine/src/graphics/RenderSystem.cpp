#include "RenderSystem.h"

#include "util\Array.h"
#include "..\log\Log.h"

#include "../Engine.h"

#include <iostream>
#include <string>
#include <fstream>

#include "../object/OBJLoader.h"

namespace Quartz
{
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

	void RenderSystem::Initialize()
	{
		GFXContext& gfx = Engine::GetInstanceHandle().GetGFXContext();
		Window& window = Engine::GetInstanceHandle().GetWindow();

		mpSurface = gfx.CreateSurface(window, window.GetWidth(), window.GetHeight(), true, false);

		Array<Byte> modelFileData = ReadFile("C:\\Development\\Quartz\\Quartz-Engine\\Source\\Sandbox\\src\\models\\bunny.obj");
		String modelDataString = String((char*)modelFileData.Data(), modelFileData.Size());

		Model model = LoadOBJ(modelDataString);

		mModel.SetTranslation({ 0.0f, 0.0f, 0.0f });
		mView.SetTranslation({ 0.0f, 0.0f, 2.0f });
		mProjection.SetIdentity();
		mProjection.SetPerspective(ToRadians(90.0f), 640.0f / 480.0f, 0.001f, 1000.0f);

		mModelUniform.AddMatrix4("model", mModel);
		mModelUniform.AddMatrix4("view", mView);
		mModelUniform.AddMatrix4("projection", mProjection);

		Array<Byte> vertexCode = ReadFile("C:\\Development\\Quartz\\Quartz-Engine\\Source\\Sandbox\\src\\vert.spv");
		GFXVertexShader* pVertexShader = gfx.CreateVertexShader(vertexCode);

		Array<Byte> pixelCode = ReadFile("C:\\Development\\Quartz\\Quartz-Engine\\Source\\Sandbox\\src\\frag.spv");
		GFXPixelShader* pPixelShader = gfx.CreatePixelShader(pixelCode);

		Array<GFXImage*> depthImages;

		GFXRenderPassInfo renderPassInfo = {};
		renderPassInfo.colorTargets.PushBack({ FORMAT_BGRA8_UNORM, GFX_LOAD_DONT_CARE, GFX_STORE_STORE }); // Swapchain image
		renderPassInfo.depthStencilTargets.PushBack({ FORMAT_DEPTH24_UNORM_STENCIL8_UINT, GFX_LOAD_CLEAR, GFX_STORE_STORE }); // depthStencil image
		GFXRenderPass* pRenderPass = gfx.CreateRenderPass(renderPassInfo);

		GFXGraphicsPipelineShaderState shaderState;
		shaderState.pVertexShader = pVertexShader;
		shaderState.pPixelShader = pPixelShader;

		mpGraphicsPipeline = gfx.CreateGraphicsPipeline(shaderState, model.vertexData.format, *pRenderPass, *mpSurface);

		mpVertexBuffer = gfx.CreateVertexBuffer(model.vertexData.buffer.Size(), true);
		void* pVertexData = gfx.MapVertexBuffer(mpVertexBuffer);
		memcpy(pVertexData, model.vertexData.buffer.Data(), mpVertexBuffer->GetSizeBytes());
		gfx.UnmapVertexBuffer(mpVertexBuffer);

		mpIndexBuffer = gfx.CreateIndexBuffer(sizeof(UInt32), model.indexData.buffer.Size() / sizeof(UInt32), true);
		void* pIndexData = gfx.MapIndexBuffer(mpIndexBuffer);
		memcpy(pIndexData, model.indexData.buffer.Data(), mpIndexBuffer->GetSizeBytes());
		gfx.UnmapIndexBuffer(mpIndexBuffer);

		for (UInt32 i = 0; i < mpSurface->GetBackBufferCount(); i++)
		{
			GFXUniformBuffer* pUniformBuffer = gfx.CreateUniformBuffer(mModelUniform.GetBuffer().Size() * sizeof(Float32), true);

			void* pUniformData = gfx.MapUniformBuffer(pUniformBuffer);
			memcpy(pUniformData, mModelUniform.GetBuffer().Data(), pUniformBuffer->GetSizeBytes());
			gfx.MapUniformBuffer(pUniformBuffer);

			mUniformBuffers.PushBack(pUniformBuffer);

			GFXImage* pDepthImage = gfx.CreateImage(IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT, mpSurface->GetWidth(), mpSurface->GetHeight(), FORMAT_DEPTH24_UNORM_STENCIL8_UINT, 1, 1);
			depthImages.PushBack(pDepthImage);

			Array<GFXImageView*> framebufferImages;
			framebufferImages.PushBack(&mpSurface->GetImageView(i));
			framebufferImages.PushBack(gfx.CreateImageView(IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT, pDepthImage, 0, 1, 0, 1));

			GFXFramebuffer* pPresentFramebuffer = gfx.CreateFramebuffer(mpGraphicsPipeline, mpSurface->GetWidth(), mpSurface->GetHeight(), framebufferImages);
			mPresentFramebuffers.PushBack(pPresentFramebuffer);

			GFXCommandBuffer* pGraphicsCommandBuffer = gfx.CreateGraphicsCommandBuffer();
			mGraphicsCommandBuffers.PushBack(pGraphicsCommandBuffer);

			pGraphicsCommandBuffer->Begin();
			gfx.BeginRenderPass(*pGraphicsCommandBuffer, *pRenderPass, *pPresentFramebuffer);

			gfx.BindGraphicsPipeline(*pGraphicsCommandBuffer, *mpGraphicsPipeline);
			gfx.BindVertexBuffer(*pGraphicsCommandBuffer, mpVertexBuffer);
			gfx.BindIndexBuffer(*pGraphicsCommandBuffer, mpIndexBuffer);
			gfx.BindUniformBuffer(*mpGraphicsPipeline, *pGraphicsCommandBuffer, pUniformBuffer);

			gfx.DrawIndexed(*pGraphicsCommandBuffer, mpIndexBuffer->GetCount(), 0);

			gfx.EndRenderPass(*pGraphicsCommandBuffer);
			pGraphicsCommandBuffer->End();
		}
	}

	void RenderSystem::Update()
	{
		GFXContext& gfx = Engine::GetInstanceHandle().GetGFXContext();

		mFrameIndex = mpSurface->AcquireNextImageIndex();

		mModel *= Matrix4().SetRotation(Quaternion().SetAxisAngle({0.0f, 1.0f, 0.0f}, 1.0f * Engine::GetInstanceHandle().GetDeltaTime()));
		mModelUniform.SetMatrix4("model", mModel);

		void* pUniformData = gfx.MapUniformBuffer(mUniformBuffers[mFrameIndex]);
		memcpy(pUniformData, mModelUniform.GetBuffer().Data(), mUniformBuffers[mFrameIndex]->GetSizeBytes());
		gfx.MapUniformBuffer(mUniformBuffers[mFrameIndex]);
		
		gfx.Submit(*mGraphicsCommandBuffers[mFrameIndex], *mpSurface);
		gfx.Present(mpSurface);
	}

	void RenderSystem::Tick()
	{

	}

	void RenderSystem::Destroy()
	{

	}
}

