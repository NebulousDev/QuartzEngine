#pragma once

#include "util/String.h"
#include "../object/RawImage.h"

namespace Quartz
{
	QUARTZ_API RawImage*	LoadImage(const String& path);
	QUARTZ_API void			FreeImage(RawImage* pImage);
}