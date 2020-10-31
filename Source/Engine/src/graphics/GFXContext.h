#pragma once

#include "Common.h"

#include "GFXResource.h"
#include "GFXSurface.h"
#include "GFXDevice.h"
#include "GFXPipelines.h"
#include "GFXShaders.h"
#include "GFXBuffers.h"
#include "GFXImage.h"
#include "GFXFramebuffer.h"
#include "GFXCommandBuffer.h"
#include "GFXRenderPass.h"
#include "GFXQueue.h"

#include "PipelineState.h"

#include "..\object\Model.h"

#include "util\Array.h"

namespace Quartz
{
	class Window;

	class QUARTZ_API GFXContext : public GFXResource
	{
	protected:
		virtual void SetDebugNameImpl(GFXResource* pResource, const String& debugName) = 0;

	public:
		virtual void CreateInstance() {};

		virtual GFXSurface* CreateSurface(Window& window, UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen) = 0;

		virtual GFXGraphicsPipeline* CreateGraphicsPipeline2(GraphicsPipelineState& state, GFXRenderPass& renderPass) = 0;

		virtual GFXGraphicsPipeline*	CreateGraphicsPipeline(
			GFXGraphicsPipelineShaderState& pipelineState,
			VertexFormat& vertexFormat,
			GFXRenderPass& renderPass,
			GFXSurface& surface, Array<GFXImageView>& imageViews
			) = 0;
		//virtual GFXComputePipelineRef	CreateComputePipeline() {};

		virtual GFXVertexShader*		CreateVertexShader(const Array<Byte>& shaderCode) = 0;
		virtual GFXPixelShader*			CreatePixelShader(const Array<Byte>& shaderCode) = 0;
		/*
		virtual GFXHullShaderRef		CreateHullShader(GFXDevice& device, const String& shaderCode) {};
		virtual GFXDomainShaderRef		CreateDomainShader(GFXDevice& device, const String& shaderCode) {};;
		virtual GFXGeometryShaderRef	CreateGeometryShader(GFXDevice& device, const String& shaderCode) {};
		virtual GFXComputeShaderRef		CreateComputeShader(GFXDevice& device, const String& shaderCode) {};
		*/

		virtual GFXVertexBuffer*		CreateVertexBuffer(UInt64 sizeBytes, Bool8 hostVisable) = 0;
		virtual GFXIndexBuffer*			CreateIndexBuffer(UInt32 stride, UInt64 size, Bool8 hostVisable) = 0;
		virtual GFXUniformBuffer*		CreateUniformBuffer(UInt32 sizeBytes, Bool8 hostVisable) = 0;
		virtual GFXTextureBuffer*		CreateTextureBuffer(UInt32 sizeBytes, Bool8 hostVisable) = 0;
		virtual GFXFramebuffer*			CreateFramebuffer(
			GFXGraphicsPipeline* pGrapicsPipeline,
			UInt32 width, UInt32 height,
			const Array<GFXImageView*>& images) = 0;

		virtual GFXCommandBuffer* CreateGraphicsCommandBuffer() = 0;
		virtual GFXCommandBuffer* CreateComputeCommandBuffer() = 0;
		virtual GFXCommandBuffer* CreateTransferCommandBuffer() = 0;

		virtual GFXImage* CreateImage(GFXImageUsage usage, UInt32 width, UInt32 height, GFXFormat format, 
			UInt32 mipLevels, UInt32 layers) = 0;
		virtual GFXImageView* CreateImageView(GFXImageUsage usage, GFXImage* pImage, UInt32 mipLevelStart, 
			UInt32 mipLevels, UInt32 layerStart, UInt32 layers) = 0;

		virtual GFXRenderPass* CreateRenderPass(GFXRenderPassInfo& renderPassInfo) = 0;

		virtual void BeginRenderPass(GFXCommandBuffer& commandBuffer, GFXRenderPass& renderPass, GFXFramebuffer& frameBuffer) = 0;
		virtual void EndRenderPass(GFXCommandBuffer& commandBuffer) = 0;

		virtual void Submit(GFXCommandBuffer& commandBuffer, GFXSurface& surface) = 0;
		virtual void Present(GFXSurface* pSurface) = 0;

		virtual void DrawIndexed(GFXCommandBuffer& commandBuffer, UInt32 indexCount, UInt32 indexOffset) = 0;

		virtual void CopyTextureBufferToImage(GFXTextureBuffer* pTextureBuffer, GFXImage* pImage) = 0;

		virtual void WaitSurfaceReady(GFXSurface& surface) = 0;

		virtual void WaitForPresent() = 0;

		virtual void BindGraphicsPipeline(GFXCommandBuffer& commandBuffer, GFXGraphicsPipeline& pipeline) = 0;
		virtual void UnbindGraphicsPipeline(GFXCommandBuffer& commandBuffer) = 0;
		
		virtual void* MapVertexBuffer(GFXVertexBuffer* pVertexBuffer) = 0;
		virtual void* MapIndexBuffer(GFXIndexBuffer* pIndexBuffer) = 0;
		virtual void* MapUniformBuffer(GFXUniformBuffer* pUniformBuffer) = 0;
		virtual void* MapTextureBuffer(GFXTextureBuffer* pTextureBuffer) = 0;

		virtual void UnmapVertexBuffer(GFXVertexBuffer* pVertexBuffer) = 0;
		virtual void UnmapIndexBuffer(GFXIndexBuffer* pIndexBuffer) = 0;
		virtual void UnmapUniformBuffer(GFXUniformBuffer* pUniformBuffer) = 0;
		virtual void UnmapTextureBuffer(GFXTextureBuffer* pTextureBuffer) = 0;

		virtual void BindVertexBuffer(GFXCommandBuffer& commandBuffer, GFXVertexBuffer* pVertexBuffer) = 0;
		virtual void BindIndexBuffer(GFXCommandBuffer& commandBuffer, GFXIndexBuffer* pIndexBuffer) = 0;
		virtual void BindUniformBuffer(GFXGraphicsPipeline& pipeline, GFXCommandBuffer& commandBuffer, GFXUniformBuffer* pUniformBuffer) = 0;
		virtual void BindTextureBuffer(GFXGraphicsPipeline& pipeline, GFXCommandBuffer& commandBuffer, GFXTextureBuffer* pTextureBuffer) = 0;

		virtual void UnbindVertexBuffer(GFXCommandBuffer& commandBuffer, GFXVertexBuffer* vertexBuffer) = 0;
		virtual void UnbindIndexBuffer(GFXCommandBuffer& commandBuffer, GFXIndexBuffer* pIndexBuffer) = 0;
		virtual void UnbindUniformBuffer(GFXCommandBuffer& commandBuffer, GFXUniformBuffer* pUniformBuffer) = 0;
		virtual void UnbindTextureBuffer(GFXCommandBuffer& commandBuffer, GFXTextureBuffer* pTextureBuffer) = 0;

		void SetDebugName(GFXResource* pResource, const String& debugName);

		virtual GFXDevice& GetDefaultDevice() = 0;
	};

}