#pragma once

#include "math/Vector3.h"

namespace Quartz
{
	struct LightComponent
	{
		Vector3 radiance;

		LightComponent(const Vector3& radiance);
	};
}