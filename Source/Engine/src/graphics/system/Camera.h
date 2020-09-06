#pragma once

#include "Common.h"
#include "math/Math.h"

namespace Quartz
{
	struct Camera
	{
		Float32	fov;
		Matrix4 projection;

		//RenderTarget target;
		//RenderRect rect;
	};
}