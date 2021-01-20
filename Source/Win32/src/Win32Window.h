#pragma once

#include "Win32.h"
#include "platform/VPPlatform.h"
#include "math/Bounds.h"

namespace Quartz
{
	struct QUARTZ_API Win32Window : public VPWindow
	{
		HWND		hwnd;
		StringW		title;
		Bounds2i	bounds;
		Bounds2i	clientBounds;
		Bool8		maximized : 1;
		Bool8		minimized : 1;
		Bool8		fullscreen : 1;
		Bool8		visible : 1;
		Bool8		mouseInside : 1;
		Bool8		focused : 1;
		Bool8		capturingMouse : 1;
	};
}