#pragma once

#include "VulkanGraphics.h"

#include "Win32Window.h"

namespace Quartz
{
	class QUARTZ_API Win32VulkanGraphics : public VulkanGraphics
	{
	public:
		Surface*	CreateSurface(Window* pWindow) override;
		void		DestroySurface(Surface* pSurface) override;
	};
}