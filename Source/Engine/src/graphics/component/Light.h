#pragma once

#include "../../entity/Component.h"
#include "math/Vector3.h"

namespace Quartz
{
	struct LightComponent : public Component<LightComponent>
	{
		Vector3 radiance;

		LightComponent(const Vector3& radiance);
	};
}