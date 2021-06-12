#pragma once

#include "../Renderer.h"
#include "../Framebuffer.h"
#include "../CommandBuffer.h"

namespace Quartz
{
	class QUARTZ_API SimpleRenderer : public Renderer
	{
	private:
		Framebuffer*	mpFramebuffer;
		CommandBuffer*	mpCommandBuffer;

	public:
		SimpleRenderer();

		void Setup(Viewport* pViewport) override;
		void Render(Viewport* pViewport, Scene* pScene) override;
		void Rebuild(Viewport* pViewport) override;
		void Destroy() override;
	};
}