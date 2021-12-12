#pragma once

#include "../Resource.h"
#include "SpirvObject.h"

namespace Quartz
{
	enum ProgramType
	{
		TRADITIONAL_PROGRAM,
		MESH_PROGRAM,
		RAYTRACING_PROGRAM,
		COMPUTE_PROGRAM,
	};

	struct QUARTZ_API Shader : public Resource<Shader>
	{
		ProgramType programType;
		Array<SpirvObject*> spirvObjects;
	};
}