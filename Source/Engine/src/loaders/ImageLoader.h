#pragma once

#include "util/String.h"
#include "../object/Image.h"

namespace Quartz
{
	QUARTZ_API Image LoadImageSTB(const String& filepath);
}