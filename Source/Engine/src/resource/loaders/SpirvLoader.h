#pragma once

#include "../object/SpirvObject.h"

namespace Quartz
{
	QUARTZ_API Bool8 LoadSpirv(const String& path, SpirvObject* pSpirvObject);
}