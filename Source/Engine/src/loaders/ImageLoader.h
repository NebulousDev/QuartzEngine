#pragma once

#include "util/String.h"
#include "../object/RawImage.h"

namespace Quartz
{
	QUARTZ_API RawImage*	LoadImage(const String& path, Bool8 flipY = false, Bool8 forceRGBA = false);
	QUARTZ_API void			FreeImage(RawImage* pImage);
}