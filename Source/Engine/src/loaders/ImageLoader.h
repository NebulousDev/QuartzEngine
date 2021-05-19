#pragma once

#include "util/String.h"
#include "../object/Image.h"

namespace Quartz
{
	QUARTZ_API Image* LoadImageFunc(const String& path);

	QUARTZ_API Bool8 UnloadImageFunc(Image* pImage);
}