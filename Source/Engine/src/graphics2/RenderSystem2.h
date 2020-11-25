#pragma once

#include "Common.h"
#include "util/RefPtr.h"

namespace Quartz
{
	
	struct FrameContext
	{
		UInt32 index;

	};

	class RenderSystem2
	{
	private:
		//GraphicsAPI* mpGfx;
		UInt32 mBackbufferCount;
		UInt32 mFrameIndex;

	public:
		// Sets the render engine to use the provided API
		// This *will* completely rebuild the pipeline
		//void SetAPI(GraphicsAPI* pGfx);
		//void SetBackbufferCount(UInt32 count);

		//void PrepareFrame();
		//void RenderFrame();
	};
}