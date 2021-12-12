#pragma once

#include "util/Array.h"
#include "../Resource.h"

namespace Quartz
{
	struct QUARTZ_API SpirvObject : public Resource<SpirvObject>
	{
		Array<Byte> binary;
	};
}