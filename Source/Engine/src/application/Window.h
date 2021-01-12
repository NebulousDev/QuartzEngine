#pragma once

#include "../platform/VPPlatform.h"
#include "../graphics/VGFXContext.h"

namespace Quartz
{
	struct QUARTZ_API GraphicsWindow
	{
		HVPWindow window;
		HGFXSurface surface;
		HGFXSwapchain swapchain;

		void Show() const;
		void Focus() const;
	};
}