#include "RenderSystem.h"

#include "util\Array.h"
#include "..\log\Log.h"

#include "../Engine.h"

#include <iostream>
#include <string>
#include <fstream>

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

		Array<Byte> vertexCode = ReadFile("C:\\Development\\Quartz\\Quartz-Engine\\Source\\Sandbox\\src\\vert.spv");
		GFXVertexShader* pVertexShader = gfx.CreateVertexShader(vertexCode);

		Array<Byte> pixelCode = ReadFile("C:\\Development\\Quartz\\Quartz-Engine\\Source\\Sandbox\\src\\frag.spv");
		GFXPixelShader* pPixelShader = gfx.CreatePixelShader(pixelCode);

		GFXRenderPassInfo renderPassInfo = {};
		renderPassInfo.colorTargets.PushBack({ FORMAT_BGRA8_UNORM, GFX_LOAD_DONT_CARE, GFX_STORE_STORE }); // Swapchain image
		GFXRenderPass* pRenderPass = gfx.CreateRenderPass(renderPassInfo);

		GFXGraphicsPipelineShaderState shaderState;
		shaderState.pVertexShader = pVertexShader;
		shaderState.pPixelShader = pPixelShader;

		mpGraphicsPipeline = gfx.CreateGraphicsPipeline(shaderState, *pRenderPass, *mpSurface);

		float positions[3 * 3] =
		{
			-0.5f,  0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			 0.0f, -0.5f, 0.0f
		};

		UInt16 indices[3] =
		{
			0, 1, 2
		};

		struct UBO
		{
			float mvp[16];
		} 
		ubo {};

		ubo.mvp[0 ] = 1.0f;
		ubo.mvp[5 ] = 1.0f;
		ubo.mvp[10] = 1.0f;
		ubo.mvp[15] = 1.0f;

		mpVertexBuffer = gfx.CreateVertexBuffer(sizeof(Float32) * 9, true);
		void* pVertexData = gfx.MapVertexBuffer(mpVertexBuffer);
		memcpy(pVertexData, positions, mpVertexBuffer->GetSizeBytes());
		gfx.UnmapVertexBuffer(mpVertexBuffer);

		mpIndexBuffer = gfx.CreateIndexBuffer(sizeof(UInt16), 3, true);
		void* pIndexData = gfx.MapIndexBuffer(mpIndexBuffer);
		memcpy(pIndexData, indices, mpIndexBuffer->GetSizeBytes());
		gfx.UnmapIndexBuffer(mpIndexBuffer);

		for (UInt32 i = 0; i < mpSurface->GetBackBufferCount(); i++)
		{
			GFXUniformBuffer* pUniformBuffer = gfx.CreateUniformBuffer(sizeof(UBO), true);

			void* pUniformData = gfx.MapUniformBuffer(pUniformBuffer);
			memcpy(pUniformData, &ubo, pUniformBuffer->GetSizeBytes());
			gfx.MapUniformBuffer(pUniformBuffer);

			mUniformBuffers.PushBack(pUniformBuffer);

			GFXFramebuffer* pPresentFramebuffer = gfx.CreateFramebuffer(mpGraphicsPipeline, mpSurface, i);
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

