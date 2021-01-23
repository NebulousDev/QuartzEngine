#pragma once

#include "../object/Model.h"
#include "util\String.h"
#include "util\StringParser.h"
#include "util\Array.h"
#include "util\Map.h"
#include "math\Math.h"

namespace Quartz
{
	// Load a Model from OBJ
	QUARTZ_API Model LoadOBJ(const String& data);
}