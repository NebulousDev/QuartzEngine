#pragma once

#include "../object/JSON.h"

namespace Quartz
{
	QUARTZ_API Bool8 LoadJSON(const Char* jsonString, JSON& json);
}