#pragma once

#include "Buffer.h"
#include "Pipeline.h"
#include "RenderPass.h"
#include "Framebuffer.h"

#include "util/Array.h"

namespace Quartz
{
	class QUARTZ_API CommandBuffer
	{
	private:
		Bool8 mRecording;

	protected:
		CommandBuffer();

	public:
		virtual void BeginRecording() = 0;
		virtual void EndRecording() = 0;

		virtual void BeginRenderpass(Renderpass* pRenderpass, Framebuffer* pFramebuffer) = 0;
		virtual void EndRenderpass() = 0;

		virtual void SetPipeline(Pipeline* pPipeline) = 0;

		virtual void SetVertexBuffers(const Array<Buffer*>& buffers) = 0;
		virtual void SetIndexBuffer(Buffer* pBuffer) = 0;

		virtual void BindUniform(UInt32 set, UInt32 binding, Buffer* pBuffer) = 0;

		virtual void DrawIndexed(UInt32 count, UInt32 start) = 0;

		FORCE_INLINE Bool8 IsRecording() const { return mRecording; }
	};
}