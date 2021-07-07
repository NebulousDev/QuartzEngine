#pragma once

#include "math/Math.h"

namespace Quartz
{
	struct TransformComponent
	{
		Vector3		position;
		Quaternion	rotation;
		Vector3		scale;

		TransformComponent();
		TransformComponent(const Vector3& position, const Quaternion& rotation, const Vector3& scale);

		Vector3 GetForward();
		Vector3 GetBackward();
		Vector3 GetLeft();
		Vector3 GetRight();
		Vector3 GetUp();
		Vector3 GetDown();

		Matrix4 GetMatrix();
	};
}