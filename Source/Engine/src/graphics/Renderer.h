#pragma once

#include "Viewport.h"
#include "SceneSystem.h"

namespace Quartz
{
	class QUARTZ_API Renderer
	{
	protected:
		Renderer();

	public:
		virtual void Setup(Context* pViewport) = 0;
		virtual void Render(Context* pViewport, Scene* pScene) = 0;
		virtual void Rebuild(Context* pViewport) = 0;
		virtual void Destroy() = 0;
	};
}