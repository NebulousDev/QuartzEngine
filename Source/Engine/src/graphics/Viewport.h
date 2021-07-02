#pragma once

#include "Surface.h"
#include "SceneSystem.h"

namespace Quartz
{
	class Renderer;

	enum MultibufferType
	{
		MULTIBUFFER_SINGLE,
		MULTIBUFFER_DOUBLE,
		MULTIBUFFER_TRIPPLE,
	};

	class QUARTZ_API Context
	{
	private:
		Surface*		mpSurface;
		Scene*			mpScene;
		Renderer*		mpRenderer;
		MultibufferType	mMultibufferType;
		UInt32			mMultibufferCount;

	public:
		Context(Surface* pSurface, Scene* pScene, Renderer* pRenderer, MultibufferType multibuffer);

		void Rebuild();

		FORCE_INLINE Surface*	GetSurface() { return mpSurface; }
		FORCE_INLINE Scene*		GetScene() { return mpScene; }
		FORCE_INLINE Renderer*	GetRenderer() { return mpRenderer; }
		FORCE_INLINE UInt32		GetWidth() { return mpSurface->GetWidth(); }
		FORCE_INLINE UInt32		GetHeight() { return mpSurface->GetHeight(); }
		FORCE_INLINE UInt32		GetMultibufferType() { return mMultibufferType; }
		FORCE_INLINE UInt32		GetMultibufferCount() { return mMultibufferCount; }
	};
}