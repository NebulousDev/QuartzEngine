#pragma once

#include "../Module.h"
#include "../platform/Window.h"
#include "Surface.h"
#include "Viewport.h"
#include "Image.h"
#include "Buffer.h"
#include "Uniform.h"
#include "Renderer.h"
#include "RenderPass.h"
#include "Framebuffer.h"
#include "Pipeline.h"
#include "Shader.h"
#include "CommandBuffer.h"

#include "util/Array.h"

namespace Quartz
{
	class QUARTZ_API Graphics : public Module
	{
	protected:
		Array<Context*> mViewports;

	protected:
		Graphics(const ModuleInfo& info);
	
	public:
		virtual Surface*		CreateSurface(Window* pWindow) = 0;
		virtual void			DestroySurface(Surface* pSurface) = 0;

		virtual Context*		CreateContext(Surface* pSurface, Scene* pScene, Renderer* pRenderer, MultibufferType multibuffer) = 0;
		virtual void			DestroyContext(Context* pContext) = 0;

		virtual Image*			CreateImage(ImageType type, UInt32 width, UInt32 height, UInt32 depth, 
									UInt32 layers, UInt32 mips, ImageFormat format, ImageUsages usages) = 0;
		virtual void			DestroyImage(Image* pImage) = 0;

		virtual ImageView*		CreateImageView(Image* pImage, ImageViewType type, UInt32 width, UInt32 height,
									UInt32 depth, UInt32 layers, UInt32 layerStart, UInt32 mips, UInt32 mipStart, 
									ImageFormat format, ImageViewUsage usage) = 0;
		virtual void			DestroyImageView(ImageView* pImageView) = 0;

		virtual Buffer*			CreateBuffer(UInt32 sizeBytes, BufferUsages usages, BufferAccess access) = 0;
		virtual void			DestroyBuffer(Buffer* pBuffer) = 0;

		virtual Uniform*		CreateUniform(UniformType type, UInt32 elementSize, UInt32 elementCount, UniformFlags flags) = 0;
		virtual void			DestroyUniform(Uniform* pUniform) = 0;

		virtual UniformTextureSampler*		CreateUniformTextureSampler() = 0;
		virtual void						DestroyUniform(UniformTextureSampler* pUniform) = 0;

		virtual Renderpass*		CreateRenderpass(const String& name, 
									const Array<Attachment>& attachments, const Array<Subpass>& subpasses) = 0;
		virtual void			DestroyRenderpass(Renderpass* pRenderpass) = 0;

		virtual Framebuffer*	CreateFramebuffer(Renderpass* pRenderpass, Context* pViewport, UInt32 width, UInt32 height) = 0;
		virtual void			DestroyFramebuffer(Framebuffer* pFramebuffer) = 0;

		virtual Shader*			CreateShader(const String& name, const Array<Byte>& binary) = 0;
		virtual void			DestroyShader(Shader* pShader) = 0;

		virtual GraphicsPipeline*	CreateGraphicsPipeline(const GraphicsPipelineInfo& info, UInt32 subpass) = 0;
		virtual void				DestroyGraphicsPipeline(GraphicsPipeline* pPipeline) = 0;

		virtual CommandBuffer*	CreateCommandBuffer(CommandBufferType type) = 0;
		virtual void			DestroyCommandBuffer(CommandBuffer* pCommandBuffer) = 0;

		virtual void			CopyBuffer(Buffer* pSource, Buffer* pDest) = 0;
		virtual void			CopyBufferToImage(Buffer* pSource, Image* pDest) = 0;

		virtual void			Submit(Context* pViewport, CommandBuffer* pCommandBuffer) = 0;
	};
}