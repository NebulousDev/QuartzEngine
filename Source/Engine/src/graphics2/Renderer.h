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
		virtual void Setup(Viewport* pViewport) = 0;
		virtual void Render(Viewport* pViewport, Scene* pScene) = 0;
		virtual void Rebuild(Viewport* pViewport) = 0;
		virtual void Destroy() = 0;
	};
}