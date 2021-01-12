#pragma once

#include "Window.h"

namespace Quartz
{
	class QUARTZ_API WindowManager
	{
	public:
		static GraphicsWindow* CreateGraphicsWindow(UInt32 posX, UInt32 posY, UInt32 width, UInt32 height, const StringW& title, UInt32 backbufferCount);
	
		static void DestroyGraphicsWindow(GraphicsWindow* pWindow);
	};
}