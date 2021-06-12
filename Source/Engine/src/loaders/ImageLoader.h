#pragma once

#include "util/String.h"
#include "../object/RawImage.h"

namespace Quartz
{
	QUARTZ_API RawImage* LoadImageFunc(const String& path);

	QUARTZ_API Bool8 UnloadImageFunc(RawImage* pImage);
}