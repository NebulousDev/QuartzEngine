#pragma once

#include "../object/GLTF.h"
#include "../object/JSON.h"

namespace Quartz
{
	QUARTZ_API Bool8 LoadGLTF(JSON& gltf, GLTFModel& gltfObject);
}