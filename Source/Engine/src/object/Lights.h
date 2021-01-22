#pragma once

#include "math/Math.h"

namespace Quartz
{
	struct DirectionalLight
	{
		Vector4 direction;
		Vector4 color;
	};

	struct PointLight
	{
		Vector4 position;
		Vector4 color;
		Vector4 attenuation;
	};
}