#pragma once

#include "../system/System.h"

namespace Quartz
{
	class QUARTZ_API RenderSystem : public System
	{
	public:
		void Initialize() override;
		void Update() override;
		void Tick() override;
		void Destroy() override;
	};
}