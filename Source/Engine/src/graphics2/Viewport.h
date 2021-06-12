#pragma once

#include "Surface.h"
#include "SceneSystem.h"

namespace Quartz
{
	class Renderer;

	class QUARTZ_API Viewport
	{
	private:
		Surface*	mpSurface;
		Scene*		mpScene;
		Renderer*	mpRenderer;

	public:
		Viewport(Surface* pSurface, Scene* pScene, Renderer* pRenderer);

		void Rebuild();

		FORCE_INLINE Surface*	GetSurface() { return mpSurface; }
		FORCE_INLINE Scene*		GetScene() { return mpScene; }
		FORCE_INLINE Renderer*	GetRenderer() { return mpRenderer; }
		FORCE_INLINE UInt32		GetWidth() { return mpSurface->GetWidth(); }
		FORCE_INLINE UInt32		GetHeight() { return mpSurface->GetHeight(); }
	};
}