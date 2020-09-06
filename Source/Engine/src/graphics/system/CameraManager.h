#pragma once

#include "Common.h"
#include "math/Math.h"
#include "util/Array.h"
#include "Camera.h"

namespace Quartz
{
	class CameraManager
	{
	private:
		Array<Camera> mActiveCameras;

	public:
		void AddActiveCamera(Camera camera) {}
	};
}