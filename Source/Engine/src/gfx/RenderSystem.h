#pragma once

#include "../system/System.h"
#include "../gfx/GraphicsContext.h"

namespace Quartz
{
	class QUARTZ_API RenderSystem : public System
	{
	private:
		GraphicsContext* mpGfx;

	public:
		void Initialize() override;
		void Update() override;
		void Tick() override;
		void Destroy() override;
	};
}