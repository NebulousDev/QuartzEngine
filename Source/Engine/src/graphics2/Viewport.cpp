#include "Viewport.h"

namespace Quartz
{
	Context::Context(Surface* pSurface, Scene* pScene, Renderer* pRenderer, MultibufferType multibuffer)
		: mpSurface(pSurface), 
		mpScene(pScene), 
		mpRenderer(pRenderer),
		mMultibufferType(multibuffer),
		mMultibufferCount((UInt32)multibuffer + 1)
	{
		// Nothing
	}

	void Context::Rebuild()
	{
		
	}
}

