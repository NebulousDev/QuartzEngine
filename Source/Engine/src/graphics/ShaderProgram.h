#pragma once

#include "Shader.h"
#include "util/Array.h"

namespace Quartz
{
	struct QUARTZ_API ShaderProgram
	{
		GFXShader* vertex;
		GFXShader* tessellationControl;
		GFXShader* tessellationEvaluation;
		GFXShader* geometry;
		GFXShader* fragment;
		GFXShader* compute;
		GFXShader* task;
		GFXShader* mesh;
		GFXShader* rayGeneration;
		GFXShader* intersection;
		GFXShader* anyHit;
		GFXShader* closestHit;
		GFXShader* miss;
		GFXShader* callable;

		Bool8 IsValidTraditional();
		Bool8 IsValidMesh();
		Bool8 IsValidCompute();
	};
}