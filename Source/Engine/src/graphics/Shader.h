#pragma once

#include "util/String.h"

namespace Quartz
{
	enum ShaderType
	{
		SHADER_VERTEX,
		SHADER_TESSELLATION_CONTROL,
		SHADER_TESSELLATION_EVALUATION,
		SHADER_GEOMETRY,
		SHADER_FRAGMENT,
		SHADER_COMPUTE,
		SHADER_KERNEL,	// Unused
		SHADER_TASK,
		SHADER_MESH,
		SHADER_RAY_GENERATION,
		SHADER_INTERSECTION,
		SHADER_ANY_HIT,
		SHADER_CLOSEST_HIT,
		SHADER_MISS,
		SHADER_CALLABLE
	};

	class QUARTZ_API GFXShader
	{
	private:
		String		mName;
		ShaderType	mType;

	protected:
		GFXShader(const String& name, ShaderType type);

	public:
		FORCE_INLINE const String&	GetName() const { return mName; }
		FORCE_INLINE ShaderType		GetType() const { return mType; }

		UInt64 Hash();
	};
}