#include "Viewport.h"

namespace Quartz
{
	Viewport::Viewport(Surface* pSurface, Scene* pScene, Renderer* pRenderer)
		: mpSurface(pSurface), mpScene(pScene), mpRenderer(pRenderer)
	{
		// Nothing
	}

	void Viewport::Rebuild()
	{
		
	}
}

