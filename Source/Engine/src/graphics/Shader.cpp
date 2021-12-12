#include "Shader.h"

namespace Quartz
{
	GFXShader::GFXShader(const String& name, ShaderType type)
		: mName(name),
		mType(type)
	{
		// Nothing
	}

	UInt64 GFXShader::Hash()
	{
		return (UInt64)mName.Hash();
	}
}

