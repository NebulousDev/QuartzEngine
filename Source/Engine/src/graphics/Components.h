#pragma once

#include "math/Math.h"
#include "VGFXContext.h"

namespace Quartz
{
	struct TransformComponent
	{
		Vector3 position;
		Quaternion rotation;
	};

	struct MeshComponent
	{
		HGFXBuffer vertexBuffer;
		HGFXBuffer indexBuffer;
		UInt32 indexStart;
		UInt32 indexEnd;
	};
}