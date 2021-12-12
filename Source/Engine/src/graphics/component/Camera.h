#pragma once

#include "../../entity/Component.h"
#include "math/Matrix4.h"

namespace Quartz
{
	struct CameraComponent : public Component<CameraComponent>
	{
		Matrix4 perspective;

		CameraComponent(const Matrix4& perspective);
	};
}