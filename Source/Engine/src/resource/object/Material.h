#pragma once

#include "Shader.h"
#include "util/Array.h"

namespace Quartz
{
	enum MaterialParameterType
	{
		MAT_PARAM_TYPE_TEXTURE,
		MAT_PARAM_TYPE_FLOAT,
		MAT_PARAM_TYPE_FLOAT2,
		MAT_PARAM_TYPE_FLOAT3,
		MAT_PARAM_TYPE_INT,
	};

	struct QUARTZ_API MaterialParameter
	{
		String name;
		MaterialParameterType type;
		
		union
		{

		};
	};

	struct QUARTZ_API Material : public Resource<Material>
	{
		String	name;
		Shader* pShader;
		Array<MaterialParameter> params;
	};
}