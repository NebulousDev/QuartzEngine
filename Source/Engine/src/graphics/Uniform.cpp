#include "Uniform.h"

namespace Quartz
{
	Uniform::Uniform(UniformType type, UInt32 elementSize, UInt32 elementCount, UniformFlags flags)
		: mType(type),
		mElementSize(elementSize),
		mElementCount(elementCount),
		mFlags(flags)
	{
		// Nothing
	}
}

